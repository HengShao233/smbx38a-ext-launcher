#include "renderInit.h"

#include "renderInternalDef.h"

namespace ExEngine::Render
{
  void InitRdDevice(SMBX::GameWnd wnd)
  {
    auto& ctx = Ctx();
    if (ctx.Inited) return;

    ctx.TargetWnd = wnd;
    HRESULT hr = CoInitialize(nullptr);

    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 120;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = ctx.TargetWnd.HWnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Windowed = TRUE;

    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1 };
    D3D_FEATURE_LEVEL featureLevel;

#ifdef _DEBUG
    UINT createDeviceFlags = 0;
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#else
    UINT createDeviceFlags = 0;
#endif
    hr = D3D11CreateDeviceAndSwapChain(
      nullptr,
      D3D_DRIVER_TYPE_HARDWARE,
      nullptr,
      createDeviceFlags,
      featureLevels,
      1,
      D3D11_SDK_VERSION,
      &swapChainDesc,
      ctx.Swapchain.GetAddressOf(),
      ctx.Dev.GetAddressOf(),
      &featureLevel,
      ctx.DevCtx.GetAddressOf()
    );
    Logger::Assert(!(FAILED(hr)), "failed to create d3d device and context and swapchain: %i", hr);

#ifdef _DEBUG
    ComPtr<ID3D11Debug> debug;
    ctx.Dev->QueryInterface(__uuidof(ID3D11Debug), (void**)&debug);
    ComPtr<ID3D11InfoQueue> infoQueue;
    debug.As(&infoQueue);
    infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
    infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
    ctx.Inited = true;
    InitRdResource();
  }
}