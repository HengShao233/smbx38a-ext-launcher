#include "winapiUtil.h"
#include "strUtil.h"

namespace ExEngine::Util
{
	std::string GetWndName(HWND hWnd)
	{
		if (!hWnd) return Empty();

        int length = GetWindowTextLength(hWnd);
		if (length <= 0) return Empty();

		auto buffer = new TCHAR[length + 2]{ '\0' };
		GetWindowText(hWnd, buffer, length + 1);
		auto s = W2S(buffer);
		delete[] buffer;
		return s;
	}

	float GetDpiScale(HWND hwnd)
	{
		return 1;
	}
}