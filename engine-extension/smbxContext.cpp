#include "util/logger.h"
#include "util/winapiUtil.h"
#include "smbxContext.h"
#include "event.h"

#include <memory>
#include <string>
#include <mutex>

#include "detours.h"

namespace ExEngine::SMBX
{
	static HWND _HWND_WAIT = NULL;

	// ��������������ж�һ�������Ƿ��� smbx38a ��������
	auto mainWindowSignature = std::string("Super Mario Bros. X by 38A - Version 1.4.5");

	static SmbxCtx& Get_Internal()
	{
		static std::unique_ptr<SmbxCtx> _CTX = std::make_unique<SmbxCtx>();
		return *_CTX;
	}
	const SmbxCtx& Get() { return Get_Internal(); }

	void __Init_WndName(const std::string& name) {
		mainWindowSignature = name;
	}

	static bool __Try_LoadWeak(HWND hWnd)
	{
		auto& ctx = Get_Internal();
		if (ctx.MainWindow) { return true; }

		if (!hWnd) return false;
		auto wndName = Util::GetWndName(hWnd);
		if (wndName != mainWindowSignature) return false;
		Logger::Info("check window: %s", wndName.c_str());
		_HWND_WAIT = hWnd;
		return true;
	}

	bool __Try_LoadWnd(HWND hWnd, int nCmdShow)
	{
		auto& ctx = Get_Internal();
		if (ctx.MainWindow) { return true; }

		if (!hWnd) return false;
		auto wndName = Util::GetWndName(hWnd);
		if (wndName != mainWindowSignature) return false;
		ctx.MainWindow = hWnd;
		HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
		ctx.Instance = hInstance;
		Logger::Info("find smbx main window success: %i, %s", hInstance, wndName.c_str());

		using namespace XD::Event;
		StaticEventMgr::broadcast<WndLoaded>(hWnd, hInstance, nCmdShow);
		return true;
	}

	void __Try_LoadWnd_Update() // ��ʱ�� smbx38a ����� showWindow ����(��֪��Ϊʲô......), ����Ϊ�˶��׽���һ����ѯ
								// �ú������� D3d9 �� Present �б�����, ��Ϊ�����ܸ� smbx ����ѭ��ͬ������
								// �ź������������Ļ�, �� D3d9 Dev ����������ǰ������治��������......�����һ��ͣ��ʱ��......
								// ���Ծ��� showWindow �������ᱻ����ԭ����˼����......
	{
		auto& ctx = Get();
		if (ctx.MainWindow) return;
		if (!_HWND_WAIT) return;
		if (__Try_LoadWnd(_HWND_WAIT, SW_SHOWNOACTIVATE) && ctx.MainWindow)
		{
			RECT rect;
			GetWindowRect(ctx.MainWindow, &rect);
			int winWidth = rect.right - rect.left;
			int winHeight = rect.bottom - rect.top;
			int screenWidth = GetSystemMetrics(SM_CXSCREEN);
			int screenHeight = GetSystemMetrics(SM_CYSCREEN);
			int xPos = (screenWidth - winWidth) / 2;
			int yPos = (screenHeight - winHeight) / 2;
			SendMessage(ctx.MainWindow, WM_MOVE, 0, MAKELPARAM(xPos, yPos));
		}
	}

	static BOOL CALLBACK FindWindowProc(HWND hwnd, LPARAM lParam)	// Ѱ��һ�����������Ĵ���
	{
		if (_HWND_WAIT) return FALSE;
		static std::chrono::time_point<std::chrono::steady_clock> start =
			std::chrono::high_resolution_clock::now();

		DWORD processId;
		GetWindowThreadProcessId(hwnd, &processId);
		if (processId == (DWORD)lParam)
		{
			if (__Try_LoadWeak(hwnd)) return FALSE;
		}
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		// ������� 20 �뻹û�ҵ��κ�һ������������ smbx ������, ��ǿ���˳�����
		if (duration.count() > 20000) { 
			Logger::Info("find window fail: %i", processId);
			ExitProcess(-4);
			return FALSE;
		}
		return TRUE;
	}

	void __LoadWindowSupplement()	// ������Ϊ��Ԥ�� smbx û���� showWindow ����������ǿ������
									// �����ý����µ����� HWND ʵ��, �ҵ� smbx ����Ϸ������
									// ����ҵ��˾ʹ��� _HWND_WAIT ��, �� smbx ��ѭ����������������� __Try_LoadWnd_Update()
	{
		if (Get().MainWindow) { return; }
		auto processId = GetCurrentProcessId();
		Logger::Info("supplement begin: %i", processId);
		while(EnumWindows(FindWindowProc, (LPARAM)processId));
	}
}
