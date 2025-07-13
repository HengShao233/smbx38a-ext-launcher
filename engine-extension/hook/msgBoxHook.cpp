#include <Windows.h>

#include <detours.h>

#include "../util/logger.h"
#include "../util/strUtil.h"

typedef int (WINAPI* MessageBoxW_t)(HWND, LPCWSTR, LPCWSTR, UINT);
MessageBoxW_t TrueMessageBoxW = MessageBoxW;

static int WINAPI HookedMessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType) {
  auto len = lstrlenW(lpText);
  if (len < 2 || lpText[0] != TEXT(':') || lpText[1] != TEXT(':')) return TrueMessageBoxW(hWnd, lpText, lpCaption, uType);

  auto content = ExEngine::Util::W2S(lpText);
  auto title = ExEngine::Util::W2S(lpCaption);

  ExEngine::Logger::Info("msgBox: %s, %s", title.c_str(), content.c_str());
  return IDOK;
}

typedef int (WINAPI* MessageBoxA_t)(HWND, LPCSTR, LPCSTR, UINT);
MessageBoxA_t TrueMessageBoxA = MessageBoxA;

static int WINAPI HookedMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {
  auto len = lstrlenA(lpText);
  if (len < 2 || lpText[0] != TEXT(':') || lpText[1] != TEXT(':')) return TrueMessageBoxA(hWnd, lpText, lpCaption, uType);

  auto content = std::string(lpText);
  auto title = std::string(lpCaption);

  ExEngine::Logger::Info("msgBox: %s, %s", title.c_str(), content.c_str());
  return IDOK;
}

#include "msgBoxHook.h"

namespace ExEngine::Hook {
  void AttachMsgBoxDetours() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)TrueMessageBoxW, (void**)HookedMessageBoxW);
    DetourTransactionCommit();

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)TrueMessageBoxA, (void**)HookedMessageBoxA);
    DetourTransactionCommit();
  }

  void DetachMsgBoxDetours() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)TrueMessageBoxA, (void**)HookedMessageBoxA);
    DetourTransactionCommit();
  }
}