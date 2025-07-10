// main.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "Windows.h"
#include "passport.h"
#include <atlstr.h>

#include <string>
#include <strsafe.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    STARTUPINFO si = { 0 };
    si.cb = sizeof(STARTUPINFO);
    PROCESS_INFORMATION pi = { 0 };

    LPTSTR lpszCurrentVariable;
    DWORD dwFlags = 0;

    LPTSTR lpEnv = GetEnvironmentStringsW();
    if (lpEnv == NULL) {
        return -3;
    }

    size_t envSize = 100;
    LPTSTR lpCurrent = lpEnv;
    while (*lpCurrent) {
        envSize += wcslen(lpCurrent) + 1;
        lpCurrent += wcslen(lpCurrent) + 1;
    }

    auto chNewEnv = new TCHAR[envSize]{ 0 };
    lpszCurrentVariable = (LPTSTR)chNewEnv;
    if (FAILED(StringCchCopy(lpszCurrentVariable, envSize, TEXT("ENGINE_PASSPORT=")))) return -3;
    lpszCurrentVariable += lstrlen(lpszCurrentVariable);
    if (FAILED(StringCchCopy(lpszCurrentVariable, envSize, PASSPORT))) return -3;
    lpszCurrentVariable += lstrlen(lpszCurrentVariable) + 1;
    *lpszCurrentVariable = (TCHAR)0;

    LPTSTR lpDest = chNewEnv;
    lpCurrent = lpEnv;
    while (*lpCurrent) {
        if (FAILED(StringCchCopy(lpszCurrentVariable, envSize, lpCurrent))) return -3;

        auto len = lstrlen(lpCurrent) + 1;
        lpCurrent += len;
        lpszCurrentVariable += len;
    }
    *lpszCurrentVariable = (TCHAR)0;

#ifdef UNICODE
    dwFlags = CREATE_UNICODE_ENVIRONMENT;
#endif
    if (!std::filesystem::exists("./assets/game/main")) {
        std::cerr << -2;
        return -2;
    }

    auto f = std::fstream("./assets/game/main", std::ios::in);
    std::string s;
    if (!std::getline(f, s)) {
        std::cerr << -3;
        return -3;
    }
    if (!std::filesystem::exists(s)) {
        std::cerr << -4;
        return -4;
    }

#ifdef UNICODE
    const auto curPath = std::filesystem::current_path().wstring() + L"/";
#else
    const auto curPath = std::filesystem::current_path().string() + "/";
#endif // UNICODE

    TCHAR launcher[] = _T("./engine/engine-ext.exe \"");
    TCHAR cmd[2048] = _T("");

    const auto cmdSize = s.size() + curPath.size() + (sizeof(launcher) / sizeof(launcher[0]));
    const auto pathSize = s.size();
    if (cmdSize >= 2047 || pathSize <= 0) {
        std::cerr << -5;
        return -5;
    }

    USES_CONVERSION;
    auto path = A2T(s.c_str());

    auto offset = 0;
    StrCpy(cmd + offset, launcher);

    offset += (sizeof(launcher) / sizeof(launcher[0])) - 1;
    StrCpy(cmd + offset, curPath.c_str());

    if (path) {
        offset += curPath.size();
        StrCpy(cmd + offset, path);
    }

    cmd[cmdSize - 1] = TCHAR('"');

    if (CreateProcess(
        NULL,
        cmd,
        NULL,
        NULL,
        false,
        dwFlags,
        (LPVOID)chNewEnv,
        TEXT("./engine"),
        &si,
        &pi
    )) return 0;

    // fail
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    std::cerr << -1;
    return -1;
}