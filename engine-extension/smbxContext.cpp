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

	// 我们暂且拿这个判定一个窗口是否是 smbx38a 的主窗口
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

	void __Try_LoadWnd_Update() // 有时候 smbx38a 不会调 showWindow 函数(不知道为什么......), 这里为了兜底进行一个轮询
								// 该函数放在 D3d9 的 Present 中被调用, 因为这样能跟 smbx 的主循环同步运行
								// 遗憾的是这样做的话, 在 D3d9 Dev 被创建出来前外挂引擎不会启动了......会带来一个停顿时间......
								// 所以具体 showWindow 函数不会被调的原因还在思考中......
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

	static BOOL CALLBACK FindWindowProc(HWND hwnd, LPARAM lParam)	// 寻找一个符合条件的窗口
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

		// 如果超过 20 秒还没找到任何一个符合条件的 smbx 主窗口, 则强制退出进程
		if (duration.count() > 20000) { 
			Logger::Info("find window fail: %i", processId);
			ExitProcess(-4);
			return FALSE;
		}
		return TRUE;
	}

	void __LoadWindowSupplement()	// 这里是为了预防 smbx 没调用 showWindow 函数而做的强制搜索
									// 遍历该进程下的所有 HWND 实例, 找到 smbx 的游戏主窗口
									// 如果找到了就存在 _HWND_WAIT 里, 由 smbx 主循环启动创建外挂引擎 __Try_LoadWnd_Update()
	{
		if (Get().MainWindow) { return; }
		auto processId = GetCurrentProcessId();
		Logger::Info("supplement begin: %i", processId);
		while(EnumWindows(FindWindowProc, (LPARAM)processId));
	}
}
