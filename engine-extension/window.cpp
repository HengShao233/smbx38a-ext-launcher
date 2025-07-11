#include "window.h"

#include "event.h"
#include "util/uuidGen.h"

#include "util/logger.h"
#include "smbxContext.h"

#include "util/winapiUtil.h"
#include "util/timeMgr.h"

namespace ExEngine
{
    static HWND _GAME_WND = NULL;
    static HWND _TEMP_MASK_WINDOW = NULL;

    static HBRUSH _hBrush = CreateSolidBrush(RGB(0, 0, 0));
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        switch (uMsg) {
        case WM_ACTIVATE:
            if (wParam != WA_INACTIVE)
            {
                auto& ctx = SMBX::Get();
                SetForegroundWindow(ctx.MainWindow);
                return 0;
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, _hBrush);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_SETCURSOR:
            if ((HWND)wParam == hwnd)
            {
                SetCursor(LoadCursor(NULL, IDC_ARROW));
                return TRUE;
            }
            break;
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    static WNDPROC _oriWndProc = NULL;
    static LRESULT CALLBACK NewWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE) {
                PostMessage(hwnd, WM_CLOSE, 0, 0);
            }
            break;

        case WM_SYSCOMMAND:
            if (wParam == SC_MOVE) return 0;
            if (wParam == SC_MAXIMIZE) return 0;
            if (wParam == SC_MINIMIZE) return 0;
            break;
        }
        return CallWindowProc(_oriWndProc, hwnd, message, wParam, lParam);
    }

    LPCTSTR SMBX_EXT_GAME_CLASSNAME = TEXT("SMBX_EXT_GAME_CLASS");
    LPCTSTR SMBX_EXT_MASK_GAME_CLASSNAME = TEXT("SMBX_EXT_MASK_GAME_CLASS");
    static bool _wndClassRegistered = false;
    static void CreateGameWnd(HWND parent, HINSTANCE hInstance, int nCmdShow)   // �����������������(ȫ������Ϸ����)
        // ���������������������ǳ��ָ���ҵ�������ͼ
    {
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        if (!_wndClassRegistered)
        {
            WNDCLASS wc = { 0 };
            wc.lpfnWndProc = WindowProc;
            wc.hInstance = hInstance;
            wc.lpszClassName = SMBX_EXT_GAME_CLASSNAME;
            RegisterClass(&wc);
            _wndClassRegistered = true;
        }

        auto w = screenWidth;
        auto h = screenHeight;

        _GAME_WND = CreateWindowEx(
            0,                                // ��չ���
            SMBX_EXT_GAME_CLASSNAME,          // ��������
            TEXT("Game"),                     // ���ڱ���
            WS_POPUP | WS_VISIBLE,            // ������ʽ
            0,
            0,                                // ��ʼλ��
            w,
            h,                                // ��С����������Ļ
            parent,                           // ������
            NULL,                             // �˵�
            hInstance,                        // ʵ�����
            NULL                              // ���Ӳ���
        );

        _oriWndProc = (WNDPROC)SetWindowLongPtr(parent, GWLP_WNDPROC, (LONG_PTR)NewWndProc);
        ShowWindow(_GAME_WND, nCmdShow);
        SetForegroundWindow(parent);

        using namespace XD::Event;

        // �������Ӵ�������ϵ��¼�
        // ������Ӧ���Ǵ��� dx11 ʵ��
        StaticEventMgr::broadcastAsync<ExWndCreated>(SMBX::GameWnd{
            .HWnd = _GAME_WND,
            .Width = w,
            .Height = h,
            .DPI = Util::GetDpiScale(_GAME_WND)
            });
	}

    static LRESULT CALLBACK WindowProcMask(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        switch (uMsg) {
        case WM_MOUSEACTIVATE:
            return MA_NOACTIVATE;
        case WM_ACTIVATE:
        case WM_DESTROY:
            break;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, _hBrush);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_SETCURSOR:
            if ((HWND)wParam == hwnd)
            {
                SetCursor(LoadCursor(NULL, IDC_ARROW));
                return TRUE;
            }
            break;
        }

        auto r = DefWindowProc(hwnd, uMsg, wParam, lParam);
        return r;
    }

	void ExEngine::Window::DoInit()
	{
		using namespace XD::Event;
		StaticEventMgr::registerEvent<WndLoaded>(XD::UUID::gen(), CreateGameWnd);

        if (_TEMP_MASK_WINDOW) return;
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        HINSTANCE hInstance = GetModuleHandle(NULL);

        WNDCLASS wc = { 0 };
        wc.lpfnWndProc = WindowProcMask;
        wc.hInstance = hInstance;
        wc.lpszClassName = SMBX_EXT_MASK_GAME_CLASSNAME;
        RegisterClass(&wc);

        _TEMP_MASK_WINDOW = CreateWindowEx(
            WS_EX_TOOLWINDOW,                 // ��չ���
            SMBX_EXT_MASK_GAME_CLASSNAME,     // ��������
            TEXT("Game"),                     // ���ڱ���
            WS_POPUP | WS_VISIBLE,            // ������ʽ
            0,
            0,                                // ��ʼλ��
            screenWidth,
            screenHeight,                     // ��С����������Ļ
            NULL,                             // ������
            NULL,                             // �˵�
            hInstance,                        // ʵ�����
            NULL                              // ���Ӳ���
        );
        if (!_TEMP_MASK_WINDOW) return;
        ShowWindow(_TEMP_MASK_WINDOW, SW_SHOW);
        SetWindowPos(_TEMP_MASK_WINDOW, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}

    void Window::__TryCloseMaskWindow()
    {
        auto& smbxCtx = SMBX::Get();
        static uint8_t tick = 4;

        if (!_GAME_WND) return;
        if (!_TEMP_MASK_WINDOW) return;
        if (tick--) return;

        if (_TEMP_MASK_WINDOW) DestroyWindow(_TEMP_MASK_WINDOW);
        _TEMP_MASK_WINDOW = NULL;
        {
            SetActiveWindow(_GAME_WND);
            SetForegroundWindow(_GAME_WND);
        }
    }
}