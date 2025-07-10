#include "pch.h"
#include "passport.h"

#include <cstdlib>

extern "C" __declspec(dllexport) void _() {}

HMODULE G_EXPANDED_ENGINE = NULL;

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        TCHAR buffer[256];
        DWORD size = GetEnvironmentVariable(TEXT("ENGINE_PASSPORT"), buffer, 256);
        if (size == 4) {
            auto res = lstrcmp(buffer, TEXT("NONE"));
            if (res) ExitProcess(-3);
        }

        else if (size > 0 && size < sizeof(buffer)) {
            auto res = lstrcmp(buffer, PASSPORT);
            if (res) ExitProcess(-1);
        }
        else ExitProcess(-2);

        if (G_EXPANDED_ENGINE) return TRUE;
        G_EXPANDED_ENGINE = LoadLibrary(TEXT("../engine-extension.dll"));
        if (!G_EXPANDED_ENGINE) return TRUE;

        typedef void(*TEngineMain)();
        auto entryFunc = (TEngineMain)GetProcAddress(G_EXPANDED_ENGINE, "engineStart");
        if (entryFunc) entryFunc();
        break;
    }
    case DLL_PROCESS_DETACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}

