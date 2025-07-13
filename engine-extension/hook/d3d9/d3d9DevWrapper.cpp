#include "d3d9DevWrapper.h"

#include <wrl.h>
#include "../..//render/render.h"

void SmbxUpdate();
namespace ExEngine::SMBX { void __Try_LoadWnd_Update(); }
namespace ExEngine::Window { void __TryCloseMaskWindow(); }

namespace ExEngine::Hook::D3D
{
	D3D9DevWrapper::D3D9DevWrapper(IDirect3DDevice9* pDirect3DDevice9, IDirect3D9* pDirect3D9, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		_direct3DDevice9 = pDirect3DDevice9;
		_direct3D9 = pDirect3D9;
	}
	D3D9DevWrapper::~D3D9DevWrapper() {}

	HRESULT D3D9DevWrapper::QueryInterface(const IID& riid, void** ppvObj) { return _direct3DDevice9->QueryInterface(riid, ppvObj); }
  ULONG D3D9DevWrapper::AddRef() { return _direct3DDevice9->AddRef(); }
  ULONG D3D9DevWrapper::Release() { return _direct3DDevice9->Release(); }

  HRESULT D3D9DevWrapper::TestCooperativeLevel() { return _direct3DDevice9->TestCooperativeLevel(); }
  UINT D3D9DevWrapper::GetAvailableTextureMem() { return _direct3DDevice9->GetAvailableTextureMem(); }
  HRESULT D3D9DevWrapper::EvictManagedResources() { return _direct3DDevice9->EvictManagedResources(); }

  HRESULT D3D9DevWrapper::GetDeviceCaps(D3DCAPS9* pCaps) { return _direct3DDevice9->GetDeviceCaps(pCaps); }
  HRESULT D3D9DevWrapper::GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE* pMode) { return _direct3DDevice9->GetDisplayMode(iSwapChain, pMode); }
  HRESULT D3D9DevWrapper::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS* pParameters) { return _direct3DDevice9->GetCreationParameters(pParameters); }
  HRESULT D3D9DevWrapper::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap) { return _direct3DDevice9->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap); }
  void D3D9DevWrapper::SetCursorPosition(int X, int Y, DWORD Flags) { _direct3DDevice9->SetCursorPosition(X, Y, Flags); }
  BOOL D3D9DevWrapper::ShowCursor(BOOL bShow) { return _direct3DDevice9->ShowCursor(bShow); }
  HRESULT D3D9DevWrapper::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain) { return _direct3DDevice9->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain); }
  HRESULT D3D9DevWrapper::GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain) { return _direct3DDevice9->GetSwapChain(iSwapChain, pSwapChain); }
  UINT D3D9DevWrapper::GetNumberOfSwapChains() { return _direct3DDevice9->GetNumberOfSwapChains(); }
  HRESULT D3D9DevWrapper::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters) { return _direct3DDevice9->Reset(pPresentationParameters); }

  HRESULT D3D9DevWrapper::GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer) { return _direct3DDevice9->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer); }
  HRESULT D3D9DevWrapper::GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus) { return _direct3DDevice9->GetRasterStatus(iSwapChain, pRasterStatus); }
  HRESULT D3D9DevWrapper::SetDialogBoxMode(BOOL bEnableDialogs) { return _direct3DDevice9->SetDialogBoxMode(bEnableDialogs); }
  void D3D9DevWrapper::SetGammaRamp(UINT iSwapChain, DWORD Flags, const D3DGAMMARAMP* pRamp) { _direct3DDevice9->SetGammaRamp(iSwapChain, Flags, pRamp); }
  void D3D9DevWrapper::GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP* pRamp) { _direct3DDevice9->GetGammaRamp(iSwapChain, pRamp); }
  HRESULT D3D9DevWrapper::CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle) { return _direct3DDevice9->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle); }
  HRESULT D3D9DevWrapper::CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle) { return _direct3DDevice9->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle); }
  HRESULT D3D9DevWrapper::CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle) { return _direct3DDevice9->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle); }
  HRESULT D3D9DevWrapper::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle) { return _direct3DDevice9->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle); }
  HRESULT D3D9DevWrapper::CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle) { return _direct3DDevice9->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle); }
  HRESULT D3D9DevWrapper::CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) { return _direct3DDevice9->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle); }
  HRESULT D3D9DevWrapper::UpdateSurface(IDirect3DSurface9* pSourceSurface, const RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, const POINT* pDestPoint) { return _direct3DDevice9->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint); }
  HRESULT D3D9DevWrapper::UpdateTexture(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture) { return _direct3DDevice9->UpdateTexture(pSourceTexture, pDestinationTexture); }
  HRESULT D3D9DevWrapper::GetRenderTargetData(IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface) { return _direct3DDevice9->GetRenderTargetData(pRenderTarget, pDestSurface); }
  HRESULT D3D9DevWrapper::GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9* pDestSurface) { return _direct3DDevice9->GetFrontBufferData(iSwapChain, pDestSurface); }
  HRESULT D3D9DevWrapper::StretchRect(IDirect3DSurface9* pSourceSurface, const RECT* pSourceRect, IDirect3DSurface9* pDestSurface, const RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter) { return _direct3DDevice9->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter); }
  HRESULT D3D9DevWrapper::ColorFill(IDirect3DSurface9* pSurface, const RECT* pRect, D3DCOLOR color) { return _direct3DDevice9->ColorFill(pSurface, pRect, color); }
  HRESULT D3D9DevWrapper::CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) { return _direct3DDevice9->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle); }
  HRESULT D3D9DevWrapper::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget) { return _direct3DDevice9->SetRenderTarget(RenderTargetIndex, pRenderTarget); }
  HRESULT D3D9DevWrapper::GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget) { return _direct3DDevice9->GetRenderTarget(RenderTargetIndex, ppRenderTarget); }
  HRESULT D3D9DevWrapper::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil) { return _direct3DDevice9->SetDepthStencilSurface(pNewZStencil); }
  HRESULT D3D9DevWrapper::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface) { return _direct3DDevice9->GetDepthStencilSurface(ppZStencilSurface); }
  HRESULT D3D9DevWrapper::BeginScene() { return _direct3DDevice9->BeginScene(); }
  HRESULT D3D9DevWrapper::EndScene() { return _direct3DDevice9->EndScene(); }
  HRESULT D3D9DevWrapper::Clear(DWORD Count, const D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) { return _direct3DDevice9->Clear(Count, pRects, Flags, Color, Z, Stencil); }
  HRESULT D3D9DevWrapper::SetTransform(D3DTRANSFORMSTATETYPE State, const D3DMATRIX* pMatrix) { return _direct3DDevice9->SetTransform(State, pMatrix); }
  HRESULT D3D9DevWrapper::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix) { return _direct3DDevice9->GetTransform(State, pMatrix); }
  HRESULT D3D9DevWrapper::MultiplyTransform(D3DTRANSFORMSTATETYPE State, const D3DMATRIX* pMatrix) { return _direct3DDevice9->MultiplyTransform(State, pMatrix); }
  HRESULT D3D9DevWrapper::SetViewport(const D3DVIEWPORT9* pViewport) { return _direct3DDevice9->SetViewport(pViewport); }
  HRESULT D3D9DevWrapper::GetViewport(D3DVIEWPORT9* pViewport) { return _direct3DDevice9->GetViewport(pViewport); }
  HRESULT D3D9DevWrapper::SetMaterial(const D3DMATERIAL9* pMaterial) { return _direct3DDevice9->SetMaterial(pMaterial); }
  HRESULT D3D9DevWrapper::GetMaterial(D3DMATERIAL9* pMaterial) { return _direct3DDevice9->GetMaterial(pMaterial); }
  HRESULT D3D9DevWrapper::SetLight(DWORD Index, const D3DLIGHT9* pLight) { return _direct3DDevice9->SetLight(Index, pLight); }
  HRESULT D3D9DevWrapper::GetLight(DWORD Index, D3DLIGHT9* pLight) { return _direct3DDevice9->GetLight(Index, pLight); }
  HRESULT D3D9DevWrapper::LightEnable(DWORD Index, BOOL Enable) { return _direct3DDevice9->LightEnable(Index, Enable); }
  HRESULT D3D9DevWrapper::GetLightEnable(DWORD Index, BOOL* pEnable) { return _direct3DDevice9->GetLightEnable(Index, pEnable); }
  HRESULT D3D9DevWrapper::SetClipPlane(DWORD Index, const float* pPlane) { return _direct3DDevice9->SetClipPlane(Index, pPlane); }
  HRESULT D3D9DevWrapper::GetClipPlane(DWORD Index, float* pPlane) { return _direct3DDevice9->GetClipPlane(Index, pPlane); }
  HRESULT D3D9DevWrapper::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value) { return _direct3DDevice9->SetRenderState(State, Value); }
  HRESULT D3D9DevWrapper::GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue) { return _direct3DDevice9->GetRenderState(State, pValue); }
  HRESULT D3D9DevWrapper::CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB) { return _direct3DDevice9->CreateStateBlock(Type, ppSB); }
  HRESULT D3D9DevWrapper::BeginStateBlock() { return _direct3DDevice9->BeginStateBlock(); }
  HRESULT D3D9DevWrapper::EndStateBlock(IDirect3DStateBlock9** ppSB) { return _direct3DDevice9->EndStateBlock(ppSB); }
  HRESULT D3D9DevWrapper::SetClipStatus(const D3DCLIPSTATUS9* pClipStatus) { return _direct3DDevice9->SetClipStatus(pClipStatus); }
  HRESULT D3D9DevWrapper::GetClipStatus(D3DCLIPSTATUS9* pClipStatus) { return _direct3DDevice9->GetClipStatus(pClipStatus); }
  HRESULT D3D9DevWrapper::GetTexture(DWORD Stage, IDirect3DBaseTexture9** ppTexture) { return _direct3DDevice9->GetTexture(Stage, ppTexture); }
  HRESULT D3D9DevWrapper::SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture) { return _direct3DDevice9->SetTexture(Stage, pTexture); }
  HRESULT D3D9DevWrapper::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue) { return _direct3DDevice9->GetTextureStageState(Stage, Type, pValue); }
  HRESULT D3D9DevWrapper::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) { return _direct3DDevice9->SetTextureStageState(Stage, Type, Value); }
  HRESULT D3D9DevWrapper::GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue) { return _direct3DDevice9->GetSamplerState(Sampler, Type, pValue); }
  HRESULT D3D9DevWrapper::SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value) { return _direct3DDevice9->SetSamplerState(Sampler, Type, Value); }
  HRESULT D3D9DevWrapper::ValidateDevice(DWORD* pNumPasses) { return _direct3DDevice9->ValidateDevice(pNumPasses); }
  HRESULT D3D9DevWrapper::SetPaletteEntries(UINT PaletteNumber, const PALETTEENTRY* pEntries) { return _direct3DDevice9->SetPaletteEntries(PaletteNumber, pEntries); }
  HRESULT D3D9DevWrapper::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries) { return _direct3DDevice9->GetPaletteEntries(PaletteNumber, pEntries); }
  HRESULT D3D9DevWrapper::SetCurrentTexturePalette(UINT PaletteNumber) { return _direct3DDevice9->SetCurrentTexturePalette(PaletteNumber); }
  HRESULT D3D9DevWrapper::GetCurrentTexturePalette(UINT* PaletteNumber) { return _direct3DDevice9->GetCurrentTexturePalette(PaletteNumber); }
  HRESULT D3D9DevWrapper::SetScissorRect(const RECT* pRect) { return _direct3DDevice9->SetScissorRect(pRect); }
  HRESULT D3D9DevWrapper::GetScissorRect(RECT* pRect) { return _direct3DDevice9->GetScissorRect(pRect); }
  HRESULT D3D9DevWrapper::SetSoftwareVertexProcessing(BOOL bSoftware) { return _direct3DDevice9->SetSoftwareVertexProcessing(bSoftware); }
  BOOL D3D9DevWrapper::GetSoftwareVertexProcessing() { return _direct3DDevice9->GetSoftwareVertexProcessing(); }
  HRESULT D3D9DevWrapper::SetNPatchMode(float nSegments) { return _direct3DDevice9->SetNPatchMode(nSegments); }
  float D3D9DevWrapper::GetNPatchMode() { return _direct3DDevice9->GetNPatchMode(); }
  HRESULT D3D9DevWrapper::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) { return _direct3DDevice9->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount); }
  HRESULT D3D9DevWrapper::DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT StartIndex, UINT PrimCount) { return _direct3DDevice9->DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, StartIndex, PrimCount); }
  HRESULT D3D9DevWrapper::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, const void* pVertexStreamZeroData, UINT VertexStreamZeroStride) { return _direct3DDevice9->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride); }
  HRESULT D3D9DevWrapper::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, const void* pIndexData, D3DFORMAT IndexDataFormat, const void* pVertexStreamZeroData, UINT VertexStreamZeroStride) { return _direct3DDevice9->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride); }
  HRESULT D3D9DevWrapper::ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags) { return _direct3DDevice9->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags); }
  HRESULT D3D9DevWrapper::CreateVertexDeclaration(const D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl) { return _direct3DDevice9->CreateVertexDeclaration(pVertexElements, ppDecl); }
  HRESULT D3D9DevWrapper::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl) { return _direct3DDevice9->SetVertexDeclaration(pDecl); }
  HRESULT D3D9DevWrapper::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl) { return _direct3DDevice9->GetVertexDeclaration(ppDecl); }
  HRESULT D3D9DevWrapper::SetFVF(DWORD FVF) { return _direct3DDevice9->SetFVF(FVF); }
  HRESULT D3D9DevWrapper::GetFVF(DWORD* pFVF) { return _direct3DDevice9->GetFVF(pFVF); }
  HRESULT D3D9DevWrapper::CreateVertexShader(const DWORD* pFunction, IDirect3DVertexShader9** ppShader) { return _direct3DDevice9->CreateVertexShader(pFunction, ppShader); }
  HRESULT D3D9DevWrapper::SetVertexShader(IDirect3DVertexShader9* pShader) { return _direct3DDevice9->SetVertexShader(pShader); }
  HRESULT D3D9DevWrapper::GetVertexShader(IDirect3DVertexShader9** ppShader) { return _direct3DDevice9->GetVertexShader(ppShader); }
  HRESULT D3D9DevWrapper::SetVertexShaderConstantF(UINT StartRegister, const float* pConstantData, UINT Vector4fCount) { return _direct3DDevice9->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount); }
  HRESULT D3D9DevWrapper::GetVertexShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount) { return _direct3DDevice9->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount); }
  HRESULT D3D9DevWrapper::SetVertexShaderConstantI(UINT StartRegister, const int* pConstantData, UINT Vector4iCount) { return _direct3DDevice9->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount); }
  HRESULT D3D9DevWrapper::GetVertexShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount) { return _direct3DDevice9->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount); }
  HRESULT D3D9DevWrapper::SetVertexShaderConstantB(UINT StartRegister, const BOOL* pConstantData, UINT BoolCount) { return _direct3DDevice9->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount); }
  HRESULT D3D9DevWrapper::GetVertexShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount) { return _direct3DDevice9->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount); }

  HRESULT D3D9DevWrapper::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride) { return _direct3DDevice9->GetStreamSource(StreamNumber, ppStreamData, pOffsetInBytes, pStride); }
  HRESULT D3D9DevWrapper::SetStreamSourceFreq(UINT StreamNumber, UINT Setting) { return _direct3DDevice9->SetStreamSourceFreq(StreamNumber, Setting); }
  HRESULT D3D9DevWrapper::GetStreamSourceFreq(UINT StreamNumber, UINT* pSetting) { return _direct3DDevice9->GetStreamSourceFreq(StreamNumber, pSetting); }
  HRESULT D3D9DevWrapper::SetIndices(IDirect3DIndexBuffer9* pIndexData) { return _direct3DDevice9->SetIndices(pIndexData); }
  HRESULT D3D9DevWrapper::GetIndices(IDirect3DIndexBuffer9** ppIndexData) { return _direct3DDevice9->GetIndices(ppIndexData); }
  HRESULT D3D9DevWrapper::CreatePixelShader(const DWORD* pFunction, IDirect3DPixelShader9** ppShader) { return _direct3DDevice9->CreatePixelShader(pFunction, ppShader); }
  HRESULT D3D9DevWrapper::SetPixelShader(IDirect3DPixelShader9* pShader) { return _direct3DDevice9->SetPixelShader(pShader); }
  HRESULT D3D9DevWrapper::GetPixelShader(IDirect3DPixelShader9** ppShader) { return _direct3DDevice9->GetPixelShader(ppShader); }
  HRESULT D3D9DevWrapper::SetPixelShaderConstantF(UINT StartRegister, const float* pConstantData, UINT Vector4fCount) { return _direct3DDevice9->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount); }
  HRESULT D3D9DevWrapper::GetPixelShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount) { return _direct3DDevice9->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount); }
  HRESULT D3D9DevWrapper::SetPixelShaderConstantI(UINT StartRegister, const int* pConstantData, UINT Vector4iCount) { return _direct3DDevice9->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount); }
  HRESULT D3D9DevWrapper::GetPixelShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount) { return _direct3DDevice9->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount); }
  HRESULT D3D9DevWrapper::SetPixelShaderConstantB(UINT StartRegister, const BOOL* pConstantData, UINT BoolCount) { return _direct3DDevice9->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount); }
  HRESULT D3D9DevWrapper::GetPixelShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount) { return _direct3DDevice9->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount); }
  HRESULT D3D9DevWrapper::DrawRectPatch(UINT Handle, const float* pNumSegs, const D3DRECTPATCH_INFO* pRectPatchInfo) { return _direct3DDevice9->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo); }
  HRESULT D3D9DevWrapper::DrawTriPatch(UINT Handle, const float* pNumSegs, const D3DTRIPATCH_INFO* pTriPatchInfo) { return _direct3DDevice9->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo); }
  HRESULT D3D9DevWrapper::DeletePatch(UINT Handle) { return _direct3DDevice9->DeletePatch(Handle); }
  HRESULT D3D9DevWrapper::CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery) { return _direct3DDevice9->CreateQuery(Type, ppQuery); }

  HRESULT D3D9DevWrapper::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride)
  {
  if (StreamNumber == 0) _stride = Stride;
  return _direct3DDevice9->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
  }

  HRESULT D3D9DevWrapper::GetDirect3D(IDirect3D9** ppD3D9)
  {
  *ppD3D9 = _direct3D9;
  return D3D_OK;
  }

  HRESULT D3D9DevWrapper::Present(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
  {
  using namespace Microsoft::WRL;
  ExEngine::SMBX::__Try_LoadWnd_Update();
  ExEngine::Window::__TryCloseMaskWindow();

  ComPtr<IDirect3DSurface9> pBackBuffer = nullptr;
  auto h = _direct3DDevice9->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, pBackBuffer.GetAddressOf());
  if (FAILED(h))
  {
    h = _direct3DDevice9->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
    SmbxUpdate();
    return h;
  }

  ExEngine::Render::SendSmbxBuffer(pBackBuffer.Get(), _direct3DDevice9);
  auto hr = _direct3DDevice9->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
  SmbxUpdate();
  return hr;
  }

  HRESULT D3D9DevWrapper::CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
  {
  auto hr = _direct3DDevice9->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
  return hr;
  }
}