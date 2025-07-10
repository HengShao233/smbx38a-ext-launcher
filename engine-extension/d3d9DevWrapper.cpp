#include "pch.h"
#include "d3d9DevWrapper.h"

#include "logger.h"
#include <wrl.h>

#include "render.h"

void SmbxUpdate();
namespace ExEngine::SMBX { void __Try_LoadWnd_Update(); }
namespace ExEngine::Window { void __TryCloseMaskWindow(); }

namespace ExEngine::Hook::D3D
{
	D3D9DevWarpper::D3D9DevWarpper(IDirect3DDevice9* pDirect3DDevice9, IDirect3D9* pDirect3D9, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		_direct3DDevice9 = pDirect3DDevice9;
		_direct3D9 = pDirect3D9;
	}
	D3D9DevWarpper::~D3D9DevWarpper() {}

	HRESULT D3D9DevWarpper::QueryInterface(const IID& riid, void** ppvObj) { return _direct3DDevice9->QueryInterface(riid, ppvObj); }
    ULONG D3D9DevWarpper::AddRef() { return _direct3DDevice9->AddRef(); }
    ULONG D3D9DevWarpper::Release() { return _direct3DDevice9->Release(); }

    HRESULT D3D9DevWarpper::TestCooperativeLevel() { return _direct3DDevice9->TestCooperativeLevel(); }
    UINT D3D9DevWarpper::GetAvailableTextureMem() { return _direct3DDevice9->GetAvailableTextureMem(); }
    HRESULT D3D9DevWarpper::EvictManagedResources() { return _direct3DDevice9->EvictManagedResources(); }

    HRESULT D3D9DevWarpper::GetDeviceCaps(D3DCAPS9* pCaps) { return _direct3DDevice9->GetDeviceCaps(pCaps); }
    HRESULT D3D9DevWarpper::GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE* pMode) { return _direct3DDevice9->GetDisplayMode(iSwapChain, pMode); }
    HRESULT D3D9DevWarpper::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS* pParameters) { return _direct3DDevice9->GetCreationParameters(pParameters); }
    HRESULT D3D9DevWarpper::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap) { return _direct3DDevice9->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap); }
    void D3D9DevWarpper::SetCursorPosition(int X, int Y, DWORD Flags) { _direct3DDevice9->SetCursorPosition(X, Y, Flags); }
    BOOL D3D9DevWarpper::ShowCursor(BOOL bShow) { return _direct3DDevice9->ShowCursor(bShow); }
    HRESULT D3D9DevWarpper::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain) { return _direct3DDevice9->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain); }
    HRESULT D3D9DevWarpper::GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain) { return _direct3DDevice9->GetSwapChain(iSwapChain, pSwapChain); }
    UINT D3D9DevWarpper::GetNumberOfSwapChains() { return _direct3DDevice9->GetNumberOfSwapChains(); }
    HRESULT D3D9DevWarpper::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters) { return _direct3DDevice9->Reset(pPresentationParameters); }

    HRESULT D3D9DevWarpper::GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer) { return _direct3DDevice9->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer); }
    HRESULT D3D9DevWarpper::GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus) { return _direct3DDevice9->GetRasterStatus(iSwapChain, pRasterStatus); }
    HRESULT D3D9DevWarpper::SetDialogBoxMode(BOOL bEnableDialogs) { return _direct3DDevice9->SetDialogBoxMode(bEnableDialogs); }
    void D3D9DevWarpper::SetGammaRamp(UINT iSwapChain, DWORD Flags, const D3DGAMMARAMP* pRamp) { _direct3DDevice9->SetGammaRamp(iSwapChain, Flags, pRamp); }
    void D3D9DevWarpper::GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP* pRamp) { _direct3DDevice9->GetGammaRamp(iSwapChain, pRamp); }
    HRESULT D3D9DevWarpper::CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle) { return _direct3DDevice9->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle); }
    HRESULT D3D9DevWarpper::CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle) { return _direct3DDevice9->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle); }
    HRESULT D3D9DevWarpper::CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle) { return _direct3DDevice9->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle); }
    HRESULT D3D9DevWarpper::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle) { return _direct3DDevice9->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle); }
    HRESULT D3D9DevWarpper::CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle) { return _direct3DDevice9->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle); }
    HRESULT D3D9DevWarpper::CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) { return _direct3DDevice9->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle); }
    HRESULT D3D9DevWarpper::UpdateSurface(IDirect3DSurface9* pSourceSurface, const RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, const POINT* pDestPoint) { return _direct3DDevice9->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint); }
    HRESULT D3D9DevWarpper::UpdateTexture(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture) { return _direct3DDevice9->UpdateTexture(pSourceTexture, pDestinationTexture); }
    HRESULT D3D9DevWarpper::GetRenderTargetData(IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface) { return _direct3DDevice9->GetRenderTargetData(pRenderTarget, pDestSurface); }
    HRESULT D3D9DevWarpper::GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9* pDestSurface) { return _direct3DDevice9->GetFrontBufferData(iSwapChain, pDestSurface); }
    HRESULT D3D9DevWarpper::StretchRect(IDirect3DSurface9* pSourceSurface, const RECT* pSourceRect, IDirect3DSurface9* pDestSurface, const RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter) { return _direct3DDevice9->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter); }
    HRESULT D3D9DevWarpper::ColorFill(IDirect3DSurface9* pSurface, const RECT* pRect, D3DCOLOR color) { return _direct3DDevice9->ColorFill(pSurface, pRect, color); }
    HRESULT D3D9DevWarpper::CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) { return _direct3DDevice9->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle); }
    HRESULT D3D9DevWarpper::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget) { return _direct3DDevice9->SetRenderTarget(RenderTargetIndex, pRenderTarget); }
    HRESULT D3D9DevWarpper::GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget) { return _direct3DDevice9->GetRenderTarget(RenderTargetIndex, ppRenderTarget); }
    HRESULT D3D9DevWarpper::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil) { return _direct3DDevice9->SetDepthStencilSurface(pNewZStencil); }
    HRESULT D3D9DevWarpper::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface) { return _direct3DDevice9->GetDepthStencilSurface(ppZStencilSurface); }
    HRESULT D3D9DevWarpper::BeginScene() { return _direct3DDevice9->BeginScene(); }
    HRESULT D3D9DevWarpper::EndScene() { return _direct3DDevice9->EndScene(); }
    HRESULT D3D9DevWarpper::Clear(DWORD Count, const D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) { return _direct3DDevice9->Clear(Count, pRects, Flags, Color, Z, Stencil); }
    HRESULT D3D9DevWarpper::SetTransform(D3DTRANSFORMSTATETYPE State, const D3DMATRIX* pMatrix) { return _direct3DDevice9->SetTransform(State, pMatrix); }
    HRESULT D3D9DevWarpper::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix) { return _direct3DDevice9->GetTransform(State, pMatrix); }
    HRESULT D3D9DevWarpper::MultiplyTransform(D3DTRANSFORMSTATETYPE State, const D3DMATRIX* pMatrix) { return _direct3DDevice9->MultiplyTransform(State, pMatrix); }
    HRESULT D3D9DevWarpper::SetViewport(const D3DVIEWPORT9* pViewport) { return _direct3DDevice9->SetViewport(pViewport); }
    HRESULT D3D9DevWarpper::GetViewport(D3DVIEWPORT9* pViewport) { return _direct3DDevice9->GetViewport(pViewport); }
    HRESULT D3D9DevWarpper::SetMaterial(const D3DMATERIAL9* pMaterial) { return _direct3DDevice9->SetMaterial(pMaterial); }
    HRESULT D3D9DevWarpper::GetMaterial(D3DMATERIAL9* pMaterial) { return _direct3DDevice9->GetMaterial(pMaterial); }
    HRESULT D3D9DevWarpper::SetLight(DWORD Index, const D3DLIGHT9* pLight) { return _direct3DDevice9->SetLight(Index, pLight); }
    HRESULT D3D9DevWarpper::GetLight(DWORD Index, D3DLIGHT9* pLight) { return _direct3DDevice9->GetLight(Index, pLight); }
    HRESULT D3D9DevWarpper::LightEnable(DWORD Index, BOOL Enable) { return _direct3DDevice9->LightEnable(Index, Enable); }
    HRESULT D3D9DevWarpper::GetLightEnable(DWORD Index, BOOL* pEnable) { return _direct3DDevice9->GetLightEnable(Index, pEnable); }
    HRESULT D3D9DevWarpper::SetClipPlane(DWORD Index, const float* pPlane) { return _direct3DDevice9->SetClipPlane(Index, pPlane); }
    HRESULT D3D9DevWarpper::GetClipPlane(DWORD Index, float* pPlane) { return _direct3DDevice9->GetClipPlane(Index, pPlane); }
    HRESULT D3D9DevWarpper::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value) { return _direct3DDevice9->SetRenderState(State, Value); }
    HRESULT D3D9DevWarpper::GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue) { return _direct3DDevice9->GetRenderState(State, pValue); }
    HRESULT D3D9DevWarpper::CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB) { return _direct3DDevice9->CreateStateBlock(Type, ppSB); }
    HRESULT D3D9DevWarpper::BeginStateBlock() { return _direct3DDevice9->BeginStateBlock(); }
    HRESULT D3D9DevWarpper::EndStateBlock(IDirect3DStateBlock9** ppSB) { return _direct3DDevice9->EndStateBlock(ppSB); }
    HRESULT D3D9DevWarpper::SetClipStatus(const D3DCLIPSTATUS9* pClipStatus) { return _direct3DDevice9->SetClipStatus(pClipStatus); }
    HRESULT D3D9DevWarpper::GetClipStatus(D3DCLIPSTATUS9* pClipStatus) { return _direct3DDevice9->GetClipStatus(pClipStatus); }
    HRESULT D3D9DevWarpper::GetTexture(DWORD Stage, IDirect3DBaseTexture9** ppTexture) { return _direct3DDevice9->GetTexture(Stage, ppTexture); }
    HRESULT D3D9DevWarpper::SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture) { return _direct3DDevice9->SetTexture(Stage, pTexture); }
    HRESULT D3D9DevWarpper::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue) { return _direct3DDevice9->GetTextureStageState(Stage, Type, pValue); }
    HRESULT D3D9DevWarpper::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) { return _direct3DDevice9->SetTextureStageState(Stage, Type, Value); }
    HRESULT D3D9DevWarpper::GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue) { return _direct3DDevice9->GetSamplerState(Sampler, Type, pValue); }
    HRESULT D3D9DevWarpper::SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value) { return _direct3DDevice9->SetSamplerState(Sampler, Type, Value); }
    HRESULT D3D9DevWarpper::ValidateDevice(DWORD* pNumPasses) { return _direct3DDevice9->ValidateDevice(pNumPasses); }
    HRESULT D3D9DevWarpper::SetPaletteEntries(UINT PaletteNumber, const PALETTEENTRY* pEntries) { return _direct3DDevice9->SetPaletteEntries(PaletteNumber, pEntries); }
    HRESULT D3D9DevWarpper::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries) { return _direct3DDevice9->GetPaletteEntries(PaletteNumber, pEntries); }
    HRESULT D3D9DevWarpper::SetCurrentTexturePalette(UINT PaletteNumber) { return _direct3DDevice9->SetCurrentTexturePalette(PaletteNumber); }
    HRESULT D3D9DevWarpper::GetCurrentTexturePalette(UINT* PaletteNumber) { return _direct3DDevice9->GetCurrentTexturePalette(PaletteNumber); }
    HRESULT D3D9DevWarpper::SetScissorRect(const RECT* pRect) { return _direct3DDevice9->SetScissorRect(pRect); }
    HRESULT D3D9DevWarpper::GetScissorRect(RECT* pRect) { return _direct3DDevice9->GetScissorRect(pRect); }
    HRESULT D3D9DevWarpper::SetSoftwareVertexProcessing(BOOL bSoftware) { return _direct3DDevice9->SetSoftwareVertexProcessing(bSoftware); }
    BOOL D3D9DevWarpper::GetSoftwareVertexProcessing() { return _direct3DDevice9->GetSoftwareVertexProcessing(); }
    HRESULT D3D9DevWarpper::SetNPatchMode(float nSegments) { return _direct3DDevice9->SetNPatchMode(nSegments); }
    float D3D9DevWarpper::GetNPatchMode() { return _direct3DDevice9->GetNPatchMode(); }
    HRESULT D3D9DevWarpper::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) { return _direct3DDevice9->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount); }
    HRESULT D3D9DevWarpper::DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT StartIndex, UINT PrimCount) { return _direct3DDevice9->DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, StartIndex, PrimCount); }
    HRESULT D3D9DevWarpper::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, const void* pVertexStreamZeroData, UINT VertexStreamZeroStride) { return _direct3DDevice9->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride); }
    HRESULT D3D9DevWarpper::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, const void* pIndexData, D3DFORMAT IndexDataFormat, const void* pVertexStreamZeroData, UINT VertexStreamZeroStride) { return _direct3DDevice9->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride); }
    HRESULT D3D9DevWarpper::ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags) { return _direct3DDevice9->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags); }
    HRESULT D3D9DevWarpper::CreateVertexDeclaration(const D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl) { return _direct3DDevice9->CreateVertexDeclaration(pVertexElements, ppDecl); }
    HRESULT D3D9DevWarpper::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl) { return _direct3DDevice9->SetVertexDeclaration(pDecl); }
    HRESULT D3D9DevWarpper::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl) { return _direct3DDevice9->GetVertexDeclaration(ppDecl); }
    HRESULT D3D9DevWarpper::SetFVF(DWORD FVF) { return _direct3DDevice9->SetFVF(FVF); }
    HRESULT D3D9DevWarpper::GetFVF(DWORD* pFVF) { return _direct3DDevice9->GetFVF(pFVF); }
    HRESULT D3D9DevWarpper::CreateVertexShader(const DWORD* pFunction, IDirect3DVertexShader9** ppShader) { return _direct3DDevice9->CreateVertexShader(pFunction, ppShader); }
    HRESULT D3D9DevWarpper::SetVertexShader(IDirect3DVertexShader9* pShader) { return _direct3DDevice9->SetVertexShader(pShader); }
    HRESULT D3D9DevWarpper::GetVertexShader(IDirect3DVertexShader9** ppShader) { return _direct3DDevice9->GetVertexShader(ppShader); }
    HRESULT D3D9DevWarpper::SetVertexShaderConstantF(UINT StartRegister, const float* pConstantData, UINT Vector4fCount) { return _direct3DDevice9->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount); }
    HRESULT D3D9DevWarpper::GetVertexShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount) { return _direct3DDevice9->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount); }
    HRESULT D3D9DevWarpper::SetVertexShaderConstantI(UINT StartRegister, const int* pConstantData, UINT Vector4iCount) { return _direct3DDevice9->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount); }
    HRESULT D3D9DevWarpper::GetVertexShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount) { return _direct3DDevice9->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount); }
    HRESULT D3D9DevWarpper::SetVertexShaderConstantB(UINT StartRegister, const BOOL* pConstantData, UINT BoolCount) { return _direct3DDevice9->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount); }
    HRESULT D3D9DevWarpper::GetVertexShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount) { return _direct3DDevice9->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount); }

    HRESULT D3D9DevWarpper::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride) { return _direct3DDevice9->GetStreamSource(StreamNumber, ppStreamData, pOffsetInBytes, pStride); }
    HRESULT D3D9DevWarpper::SetStreamSourceFreq(UINT StreamNumber, UINT Setting) { return _direct3DDevice9->SetStreamSourceFreq(StreamNumber, Setting); }
    HRESULT D3D9DevWarpper::GetStreamSourceFreq(UINT StreamNumber, UINT* pSetting) { return _direct3DDevice9->GetStreamSourceFreq(StreamNumber, pSetting); }
    HRESULT D3D9DevWarpper::SetIndices(IDirect3DIndexBuffer9* pIndexData) { return _direct3DDevice9->SetIndices(pIndexData); }
    HRESULT D3D9DevWarpper::GetIndices(IDirect3DIndexBuffer9** ppIndexData) { return _direct3DDevice9->GetIndices(ppIndexData); }
    HRESULT D3D9DevWarpper::CreatePixelShader(const DWORD* pFunction, IDirect3DPixelShader9** ppShader) { return _direct3DDevice9->CreatePixelShader(pFunction, ppShader); }
    HRESULT D3D9DevWarpper::SetPixelShader(IDirect3DPixelShader9* pShader) { return _direct3DDevice9->SetPixelShader(pShader); }
    HRESULT D3D9DevWarpper::GetPixelShader(IDirect3DPixelShader9** ppShader) { return _direct3DDevice9->GetPixelShader(ppShader); }
    HRESULT D3D9DevWarpper::SetPixelShaderConstantF(UINT StartRegister, const float* pConstantData, UINT Vector4fCount) { return _direct3DDevice9->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount); }
    HRESULT D3D9DevWarpper::GetPixelShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount) { return _direct3DDevice9->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount); }
    HRESULT D3D9DevWarpper::SetPixelShaderConstantI(UINT StartRegister, const int* pConstantData, UINT Vector4iCount) { return _direct3DDevice9->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount); }
    HRESULT D3D9DevWarpper::GetPixelShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount) { return _direct3DDevice9->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount); }
    HRESULT D3D9DevWarpper::SetPixelShaderConstantB(UINT StartRegister, const BOOL* pConstantData, UINT BoolCount) { return _direct3DDevice9->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount); }
    HRESULT D3D9DevWarpper::GetPixelShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount) { return _direct3DDevice9->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount); }
    HRESULT D3D9DevWarpper::DrawRectPatch(UINT Handle, const float* pNumSegs, const D3DRECTPATCH_INFO* pRectPatchInfo) { return _direct3DDevice9->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo); }
    HRESULT D3D9DevWarpper::DrawTriPatch(UINT Handle, const float* pNumSegs, const D3DTRIPATCH_INFO* pTriPatchInfo) { return _direct3DDevice9->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo); }
    HRESULT D3D9DevWarpper::DeletePatch(UINT Handle) { return _direct3DDevice9->DeletePatch(Handle); }
    HRESULT D3D9DevWarpper::CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery) { return _direct3DDevice9->CreateQuery(Type, ppQuery); }

    HRESULT D3D9DevWarpper::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride)
    {
        if (StreamNumber == 0) _stride = Stride;
        return _direct3DDevice9->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
    }

    HRESULT D3D9DevWarpper::GetDirect3D(IDirect3D9** ppD3D9)
    {
        *ppD3D9 = _direct3D9;
        return D3D_OK;
    }

    HRESULT D3D9DevWarpper::Present(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
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

    HRESULT D3D9DevWarpper::CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
    {
        auto hr = _direct3DDevice9->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
        return hr;
    }
}