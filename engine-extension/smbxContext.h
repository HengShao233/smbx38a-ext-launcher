#pragma once

#include <Windows.h>

namespace ExEngine::SMBX
{
	struct SmbxCtx
	{
		HWND MainWindow = NULL;
		HINSTANCE Instance = NULL;
	};

	const SmbxCtx& Get();

	struct GameWnd
	{
		HWND HWnd;

		int Width;
		int Height;
		float DPI;
	};

	void __Try_LoadWnd_Update();
}