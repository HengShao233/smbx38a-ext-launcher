#include "d3d9Wrapper.h"
#include "d3d9DevWrapper.h"

namespace ExEngine::Hook::D3D
{
	D3D9Warpper::D3D9Warpper(LPDIRECT3D9 pDirect3D) { _direct3D9 = pDirect3D; }
	D3D9Warpper::~D3D9Warpper() {}

  HRESULT D3D9Warpper::QueryInterface(const IID& riid, void** ppvObj) { return _direct3D9->QueryInterface(riid, ppvObj); }
  ULONG D3D9Warpper::AddRef() { return _direct3D9->AddRef(); }
	ULONG D3D9Warpper::Release() { return _direct3D9->AddRef(); }

  HRESULT D3D9Warpper::RegisterSoftwareDevice(void* pInitializeFunction) { return _direct3D9->RegisterSoftwareDevice(pInitializeFunction); }
  UINT D3D9Warpper::GetAdapterCount() { return _direct3D9->GetAdapterCount(); }
  HRESULT D3D9Warpper::GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier) { return _direct3D9->GetAdapterIdentifier(Adapter, Flags, pIdentifier); }
  UINT D3D9Warpper::GetAdapterModeCount(UINT Adapter, D3DFORMAT Format) { return _direct3D9->GetAdapterModeCount(Adapter, Format); }
  HRESULT D3D9Warpper::EnumAdapterModes(UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode) { return _direct3D9->EnumAdapterModes(Adapter, Format, Mode, pMode); }
  HRESULT D3D9Warpper::GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE* pMode) { return _direct3D9->GetAdapterDisplayMode(Adapter, pMode); }
  HRESULT D3D9Warpper::CheckDeviceType(UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed) { return _direct3D9->CheckDeviceType(Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed); }
  HRESULT D3D9Warpper::CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat) { return _direct3D9->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat); }
  HRESULT D3D9Warpper::CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels) { return _direct3D9->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels); }
  HRESULT D3D9Warpper::CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat) { return _direct3D9->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat); }
  HRESULT D3D9Warpper::CheckDeviceFormatConversion(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat) { return _direct3D9->CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat); }
  HRESULT D3D9Warpper::GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps) { return _direct3D9->GetDeviceCaps(Adapter, DeviceType, pCaps); }
  HMONITOR D3D9Warpper::GetAdapterMonitor(UINT Adapter) { return _direct3D9->GetAdapterMonitor(Adapter); }

  HRESULT D3D9Warpper::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface)
  {
    IDirect3DDevice9* pDirect3DDevice9;
    HRESULT hRes = _direct3D9->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, &pDirect3DDevice9);
    *ppReturnedDeviceInterface = new D3D9DevWarpper(pDirect3DDevice9, this, pPresentationParameters);
    return hRes;
  }
}