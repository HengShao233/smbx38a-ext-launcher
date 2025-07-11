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

static BOOL WINAPI HookedShowWindow(HWND hWnd, int nCmdShow)	// 尝试在 showWindow 时直接判断当前 window 是不是 smbx 的主窗口
																// 但是有时候(程序在某路径第一次启动时)这个函数并不会被调用
																// 不知道为什么......具体还在思考中
																// 由于上述问题, 在 smbxContext.cpp 中我还另外做了兜底
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