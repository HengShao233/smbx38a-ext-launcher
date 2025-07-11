#include "showWindowHook.h"

#include <Windows.h>
#include "detours.h"
#include "../util/logger.h"
#include "../util/strUtil.h"
#include "../util/winapiUtil.h"

using namespace ExEngine;
namespace ExEngine::SMBX { bool __Try_LoadWnd(HWND hWnd, int cmd); }

typedef BOOL(WINAPI* ShowWindow_t)(HWND hWnd, int nCmdShow);
ShowWindow_t TrueShowWindow = ShowWindow;

typedef BOOL(WINAPI* SetWindowName_t)(HWND hWnd, LPCTSTR text);
SetWindowName_t TrueSetWindowText = SetWindowText;

static BOOL WINAPI HookedShowWindow(HWND hWnd, int nCmdShow)	// ������ showWindow ʱֱ���жϵ�ǰ window �ǲ��� smbx ��������
																// ������ʱ��(������ĳ·����һ������ʱ)������������ᱻ����
																// ��֪��Ϊʲô......���廹��˼����
																// ������������, �� smbxContext.cpp ���һ��������˶���
{
	SMBX::__Try_LoadWnd(hWnd, nCmdShow);
	return TrueShowWindow(hWnd, nCmdShow);
}


static BOOL WINAPI HookedSetWindowText(HWND hWnd, LPCTSTR text) { return TrueSetWindowText(hWnd, text); }

namespace ExEngine::Hook
{
	void AttachShowWindowDetours()
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)TrueShowWindow, HookedShowWindow);
		auto hr = DetourTransactionCommit();

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)TrueSetWindowText, HookedSetWindowText);
		DetourTransactionCommit();
	}

	void DetachShowWindowDetours()
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)TrueShowWindow, HookedShowWindow);
		DetourTransactionCommit();

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)TrueSetWindowText, HookedSetWindowText);
		DetourTransactionCommit();
	}
}