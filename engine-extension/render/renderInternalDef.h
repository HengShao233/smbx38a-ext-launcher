#pragma once

#include "render.h"
#include "../util/logger.h"
#include "../util/uuidGen.h"
#include "../smbxContext.h"

#include <wrl.h>
#include <dxgi.h>
#include <d3d11.h>
#include <windows.h>

#include <d3dcompiler.h>

#include <set>
#include <queue>
#include <mutex>
#include <memory>
#include <thread>

#define SMBX_RENDER_TEXTURE_POOL_CNT 16

namespace ExEngine::Render
{
	template<typename T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#pragma region context
	struct Dx11Context
	{
		SMBX::GameWnd TargetWnd = {};
		ComPtr<ID3D11Device> Dev = nullptr;
		ComPtr<ID3D11DeviceContext> DevCtx = nullptr;
		ComPtr<IDXGISwapChain> Swapchain = nullptr;

		std::atomic_bool Inited = false;
	};
	Dx11Context& Ctx();
#pragma endregion

#pragma region resources
	struct Dx11Resource
	{
		class Shader
		{
		public:
			virtual ~Shader() = default;

			enum class EType : uint8_t
			{
				Undefine = 0,

				Vertex,
				Pixel
			};

			const EType Type = EType::Undefine;
			Shader() : Type(EType::Undefine) {}
			Shader(Shader::EType type) : Type(type) {}
			ID3DBlob* GetBlob();
			virtual operator bool() const = 0;

		protected:
			ComPtr<ID3DBlob> _blob = nullptr;
		};

		class PixelShader final : public Shader
		{
		public:
			PixelShader() = default;
			PixelShader(const char* path);
			ID3D11PixelShader* Get();
			operator bool() const override;
		private:
			ComPtr<ID3D11PixelShader> _data = nullptr;
		};

		class VertexShader final : public Shader
		{
		public:
			VertexShader() = default;
			VertexShader(const char* path);
			ID3D11VertexShader* Get();
			operator bool() const override;
		private:
			ComPtr<ID3D11VertexShader> _data = nullptr;
		};

		ComPtr<ID3D11RenderTargetView> RenderTargetView = nullptr;

		ComPtr<ID3D11Texture2D> DepthStencilBuffer = nullptr;
		ComPtr<ID3D11DepthStencilView> DepthStencilView = nullptr;

		D3D11_VIEWPORT Viewport = {};
		ComPtr<ID3D11BlendState> BlendState = nullptr;
		ComPtr<ID3D11DepthStencilState> DepthStencilState = nullptr;
		ComPtr<ID3D11RasterizerState> RasterizerState = nullptr;

		ComPtr<ID3D11InputLayout> InputLayout = nullptr;
		std::unique_ptr<PixelShader> BasePixelShader;
		std::unique_ptr<VertexShader> BaseVertexShader;

		std::unordered_map<std::string, std::unique_ptr<Shader>> UnivShaders;

		ComPtr<ID3D11Texture2D> SmbxRenderTex = nullptr;
		ComPtr<ID3D11ShaderResourceView> SmbxRenderTexView = nullptr;
		ComPtr<IDirect3DSurface9> SmbxCopyTarget = nullptr;
		ComPtr<ID3D11SamplerState> SmbxRenderDataSampler_Liner = nullptr;

		HANDLE WaitCopyToDx11 = NULL;
		std::mutex SmbxTargetWrite;
		std::atomic_bool CopyTarget = false;

		std::atomic_bool Inited = false;

		static constexpr int SMBX_TEX_WIDTH = 800;
		static constexpr int SMBX_TEX_HEIGHT = 600;
	};
	Dx11Resource& Res();

	void SendSmbxBuffer_Internal();
#pragma endregion
}