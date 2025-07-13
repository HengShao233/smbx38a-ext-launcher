#include "d3dHook.h"
#include "d3d9Wrapper.h"

#include <detours.h>

ExEngine::Hook::D3D::D3D9Wrapper* G_D3D9 = NULL;

typedef IDirect3D9* (APIENTRY* Direct3DCreate9_t)(UINT);
Direct3DCreate9_t TrueDirect3DCreate9 = nullptr;

static IDirect3D9* APIENTRY HookedDirect3DCreate9(UINT SDKVersion)
{
  auto inst = TrueDirect3DCreate9(SDKVersion);
  return G_D3D9 = new ExEngine::Hook::D3D::D3D9Wrapper(inst);
}

namespace ExEngine::Hook
{
  void AttachD3dDetours()
  {
    HMODULE hD3D9 = GetModuleHandle(TEXT("d3d9.dll"));
    if (hD3D9) {
      TrueDirect3DCreate9 = (Direct3DCreate9_t)GetProcAddress(hD3D9, "Direct3DCreate9");

      DetourTransactionBegin();
      DetourUpdateThread(GetCurrentThread());
      DetourAttach(&(PVOID&)TrueDirect3DCreate9, (void**)HookedDirect3DCreate9);
      DetourTransactionCommit();
    }
  }

  void DetachD3dDetours()
  {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)TrueDirect3DCreate9, (void**)HookedDirect3DCreate9);
    DetourTransactionCommit();
  }
}