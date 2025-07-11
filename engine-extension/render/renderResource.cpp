#include "renderInit.h"

#include "renderInternalDef.h"

namespace ExEngine::Render
{
	ID3DBlob* Dx11Resource::Shader::GetBlob() { return _blob.Get(); }

	Dx11Resource::PixelShader::PixelShader(const char* path):
		Shader(EType::Pixel)
	{
		auto& ctx = Ctx();
		if (!ctx.Inited) return;

		std::string s(path);
		std::wstring wpath = std::wstring(s.begin(), s.end());
		HRESULT hr = D3DReadFileToBlob(wpath.c_str(), _blob.GetAddressOf());
		if (FAILED(hr)) {
			Logger::Error("failed to load pixel shader bytecode");
			return;
		}

		hr = ctx.Dev->CreatePixelShader(_blob->GetBufferPointer(), _blob->GetBufferSize(), nullptr, _data.GetAddressOf());
		if (FAILED(hr)) {
			_data = nullptr;
			Logger::Error("failed to create pixel shader");
			return;
		}
	}

	Dx11Resource::PixelShader::operator bool() const { return (bool)_data; }
	ID3D11PixelShader* Dx11Resource::PixelShader::Get() { return _data.Get(); }

	Dx11Resource::VertexShader::VertexShader(const char* path) :
		Shader(EType::Vertex)
	{
		auto& ctx = Ctx();
		if (!ctx.Inited) return;

		std::string s(path);
		std::wstring wpath = std::wstring(s.begin(), s.end());
		HRESULT hr = D3DReadFileToBlob(wpath.c_str(), _blob.GetAddressOf());
		if (FAILED(hr)) {
			Logger::Error("failed to load pixel shader bytecode");
			return;
		}

		hr = ctx.Dev->CreateVertexShader(_blob->GetBufferPointer(), _blob->GetBufferSize(), nullptr, _data.GetAddressOf());
		if (FAILED(hr)) {
			_data = nullptr;
			Logger::Error("failed to create pixel shader");
			return;
		}
	}

	Dx11Resource::VertexShader::operator bool() const { return (bool)_data; }
	ID3D11VertexShader* Dx11Resource::VertexShader::Get() { return _data.Get(); }

	void SendSmbxBuffer(IDirect3DSurface9* surface, IDirect3DDevice9* dev)
	{
		auto& ctx = Ctx();
		auto& res = Res();
		if (!ctx.Inited) return;
		if (!res.Inited) return;
		if (!surface) return;

		if (!res.SmbxCopyTarget)
		{
			// 由于 D3d9 后缓冲 surface 并不在系统内存中可访问, 所以我们需要一张 D3DPOOL_SYSTEMMEM 申请出来的缓冲
			// 然后将后缓冲 surface cpoy 到申请出来的缓冲中
			auto hr = dev->CreateOffscreenPlainSurface(
				res.SMBX_TEX_WIDTH,
				res.SMBX_TEX_HEIGHT,
				D3DFMT_A8R8G8B8,
				D3DPOOL_SYSTEMMEM,
				res.SmbxCopyTarget.GetAddressOf(),
				nullptr);
			if (FAILED(hr)) {
				if (FAILED(hr)) {
					Logger::RuntimeLog("failed to create smbx render copy target");
					return;
				}
			}
		}

		{
			auto hr = dev->GetRenderTargetData(surface, res.SmbxCopyTarget.Get());
			if (FAILED(hr))
			{
				Logger::RuntimeLog("failed to stretch smbx surface: %i", hr);
				return;
			}
		}

		// 这个地方要强制两个线程同步, 因为 dx11 和 dx9 的 dev 都是线程不安全的
		// 如果不强制同步程序就会闪退
		res.CopyTarget = true;
		WaitForSingleObject(res.WaitCopyToDx11, INFINITE);
		res.CopyTarget = false;
	}

	void SendSmbxBuffer_Internal()
	{
		auto& ctx = Ctx();
		auto& res = Res();
		if (!ctx.Inited) return;
		if (!res.Inited) return;
		if (!res.CopyTarget) return;

		D3DLOCKED_RECT lockedRect;
		auto hr = res.SmbxCopyTarget->LockRect(&lockedRect, nullptr, D3DLOCK_READONLY);
		if (FAILED(hr))
		{
			Logger::RuntimeLog("failed to lock smbx data: %i", hr);
			return;
		}

		size_t rowPitch = lockedRect.Pitch;
		void* pData = lockedRect.pBits;

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		hr = ctx.DevCtx->Map(res.SmbxRenderTex.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr)) {
			Logger::RuntimeLog("failed to map smbx data");
			return;
		}

		void* pDest = mappedResource.pData;
		size_t destRowPitch = mappedResource.RowPitch;
		
		for (size_t y = 0; y < res.SMBX_TEX_HEIGHT; ++y) {
			memcpy(static_cast<char*>(pDest) + y * destRowPitch,
				static_cast<const char*>(pData) + y * rowPitch,
				(res.SMBX_TEX_WIDTH) * 4);
		}
		ctx.DevCtx->Unmap(res.SmbxRenderTex.Get(), 0);
		res.SmbxCopyTarget->UnlockRect();

		// 这个地方要强制两个线程同步, 因为 dx11 和 dx9 的 dev 都是线程不安全的
		// 如果不强制同步程序就会闪退
		SetEvent(res.WaitCopyToDx11);
	}

	static void CreateSwapchain(Dx11Context& ctx, Dx11Resource& res);
	static void CreatePipelineState(Dx11Context& ctx, Dx11Resource& res);
	static void CreateBaseShader(Dx11Context& ctx, Dx11Resource& res);
	static void CreateSmbxShareTarget(Dx11Context& ctx, Dx11Resource& res);

	void InitRdResource()
	{
		auto &ctx = Ctx();
		if (!ctx.Inited) return;

		auto &res = Res();
		CreateSwapchain(ctx, res);
		CreateBaseShader(ctx, res);
		CreateSmbxShareTarget(ctx, res);

		res.WaitCopyToDx11 = CreateEvent(NULL, FALSE, FALSE, NULL);
		res.Inited = true;
	}

	static void CreateSwapchain(Dx11Context &ctx, Dx11Resource &res)
	{
		ID3D11Texture2D* backBuffer = nullptr;
		auto hr = ctx.Swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
		Logger::Assert(!(FAILED(hr)), "failed to get back buffer");
		if (FAILED(hr)) return;

		hr = ctx.Dev->CreateRenderTargetView(backBuffer, nullptr, res.RenderTargetView.GetAddressOf());
		backBuffer->Release();
		Logger::Assert(!(FAILED(hr)), "failed to create render target view");

		D3D11_TEXTURE2D_DESC depthStencilDesc = {};
		depthStencilDesc.Width = ctx.TargetWnd.Width;
		depthStencilDesc.Height = ctx.TargetWnd.Height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		hr = ctx.Dev->CreateTexture2D(&depthStencilDesc, nullptr, res.DepthStencilBuffer.GetAddressOf());
		Logger::Assert(!(FAILED(hr)), "failed to create depth stencil buffer");

		hr = ctx.Dev->CreateDepthStencilView(res.DepthStencilBuffer.Get(), nullptr, res.DepthStencilView.GetAddressOf());
		Logger::Assert(!(FAILED(hr)), "failed to create depth stencil view");

		res.Viewport.TopLeftX = 0.0f;
		res.Viewport.TopLeftY = 0.0f;
		res.Viewport.Width = (float)ctx.TargetWnd.Width;
		res.Viewport.Height = (float)ctx.TargetWnd.Height;
		res.Viewport.MinDepth = 0.0f;
		res.Viewport.MaxDepth = 1.0f;
	}

	static void CreatePipelineState(Dx11Context& ctx, Dx11Resource& res)
	{
		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;

		D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
		renderTargetBlendDesc.BlendEnable = TRUE;
		renderTargetBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		renderTargetBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		renderTargetBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
		renderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
		renderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
		renderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		renderTargetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		blendDesc.RenderTarget[0] = renderTargetBlendDesc;

		HRESULT hr = ctx.Dev->CreateBlendState(&blendDesc, res.BlendState.GetAddressOf());
		Logger::Assert(!(FAILED(hr)), "failed to create blend state");

		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
		depthStencilDesc.DepthEnable = FALSE;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.StencilEnable = FALSE;
		depthStencilDesc.StencilReadMask = 0;
		depthStencilDesc.StencilWriteMask = 0;
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		hr = ctx.Dev->CreateDepthStencilState(&depthStencilDesc, res.DepthStencilState.GetAddressOf());
		Logger::Assert(!(FAILED(hr)), "failed to create depth stencil state");

		D3D11_RASTERIZER_DESC rasterizerDesc = {};
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		rasterizerDesc.FrontCounterClockwise = FALSE;
		rasterizerDesc.DepthBias = 0;
		rasterizerDesc.DepthBiasClamp = 0.0f;
		rasterizerDesc.SlopeScaledDepthBias = 0.0f;
		rasterizerDesc.DepthClipEnable = TRUE;
		rasterizerDesc.ScissorEnable = FALSE;
		rasterizerDesc.MultisampleEnable = FALSE;
		rasterizerDesc.AntialiasedLineEnable = FALSE;

		hr = ctx.Dev->CreateRasterizerState(&rasterizerDesc, res.RasterizerState.GetAddressOf());
		Logger::Assert(!(FAILED(hr)), "failed to create rasterizer state");
	}

	static void CreateBaseShader(Dx11Context &ctx, Dx11Resource &res)	// 这里我们先加载两个基础的 shader
																		// 主要是为了能够渲染一张矩形贴图
	{
		// 这俩 shader 是着色器筛选器里编译出来的
		auto& ps = *(res.BasePixelShader = std::make_unique<Dx11Resource::PixelShader>("../assets/shaders/pixel.cso"));
		auto& vs = *(res.BaseVertexShader = std::make_unique<Dx11Resource::VertexShader>("../assets/shaders/vertex.cso"));

		Logger::Assert(ps, "failed to create base ps");
		Logger::Assert(vs, "failed to create base vs");

		D3D11_INPUT_ELEMENT_DESC layout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		Logger::Assert(vs.GetBlob(), "vs blob empty");
		HRESULT hr = ctx.Dev->CreateInputLayout(layout, ARRAYSIZE(layout), vs.GetBlob()->GetBufferPointer(), vs.GetBlob()->GetBufferSize(), res.InputLayout.GetAddressOf());
		if (FAILED(hr)) {
			Logger::Error("failed to create input layout");
			return;
		}
	}

	static void CreateSmbxShareTarget(Dx11Context& ctx, Dx11Resource& res)	// 这是用于存放 smbx38a 渲染结果的 dx11 贴图
	{
		D3D11_TEXTURE2D_DESC texDesc = {};
		texDesc.Width = res.SMBX_TEX_WIDTH;
		texDesc.Height = res.SMBX_TEX_HEIGHT;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.Usage = D3D11_USAGE_DYNAMIC;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		auto hr = ctx.Dev->CreateTexture2D(&texDesc, nullptr, res.SmbxRenderTex.GetAddressOf());
		if (FAILED(hr)) {
			Logger::RuntimeLog("failed to create smbx render target");
			return;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = texDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

		// 创建纹理视图
		hr = ctx.Dev->CreateShaderResourceView(res.SmbxRenderTex.Get(), &srvDesc, res.SmbxRenderTexView.GetAddressOf());
		if (FAILED(hr)) {
			Logger::RuntimeLog("failed to create smbx render target tex view");
			return;
		}

		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		samplerDesc.BorderColor[0] = 1.0f;
		samplerDesc.BorderColor[1] = 1.0f;
		samplerDesc.BorderColor[2] = 1.0f;
		samplerDesc.BorderColor[3] = 1.0f;

		hr = ctx.Dev->CreateSamplerState(&samplerDesc, res.SmbxRenderDataSampler_Liner.GetAddressOf());
		if (FAILED(hr)) {
			if (FAILED(hr)) {
				Logger::RuntimeLog("failed to create smbx render target sempler");
				return;
			}
		}
	}
}