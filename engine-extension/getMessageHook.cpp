#include "pch.h"
#include "getMessageHook.h"

#include <fstream>
#include <detours.h>

#include "logger.h"
#include "strUtil.h"

typedef int (WINAPI* GetMesage_t)(
    _Out_ LPMSG lpMsg,
    _In_opt_ HWND hWnd,
    _In_ UINT wMsgFilterMin,
    _In_ UINT wMsgFilterMax);
GetMesage_t TrueGetMesage = GetMessage;

typedef BOOL (WINAPI* PeekMessage_t)(
    _Out_ LPMSG lpMsg,
    _In_opt_ HWND hWnd,
    _In_ UINT wMsgFilterMin,
    _In_ UINT wMsgFilterMax,
    _In_ UINT wRemoveMsg);
PeekMessage_t TruePeekMessage = PeekMessage;

static int WINAPI HookedGetMesage(
    _Out_ LPMSG lpMsg,
    _In_opt_ HWND hWnd,
    _In_ UINT wMsgFilterMin,
    _In_ UINT wMsgFilterMax
) {
    return TrueGetMesage(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax);
}

bool _INITED = false;
bool _ENTER_HOOKED = false;
static int WINAPI HookedPeekMessage(
    _Out_ LPMSG lpMsg,
    _In_opt_ HWND hWnd,
    _In_ UINT wMsgFilterMin,
    _In_ UINT wMsgFilterMax,
    _In_ UINT wRemoveMsg
) {
    return TruePeekMessage(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
}

void ExEngine::Hook::AttachGetMessageDetours()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)TrueGetMesage, HookedGetMesage);
    DetourTransactionCommit();

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)TruePeekMessage, HookedPeekMessage);
    DetourTransactionCommit();
}

void ExEngine::Hook::DetachGetMessageDetours()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)TrueGetMesage, HookedGetMesage);
    DetourTransactionCommit();

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)TruePeekMessage, HookedPeekMessage);
    DetourTransactionCommit();
}
