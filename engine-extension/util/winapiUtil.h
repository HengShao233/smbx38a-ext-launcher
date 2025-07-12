#pragma once

#include <string>
#include <Windows.h>

namespace ExEngine::Util
{
  /// <summary>
  ///
  /// </summary>
  /// <param name="hWnd"></param>
  /// <returns></returns>
  std::string GetWndName(HWND hWnd);

  float GetDpiScale(HWND hwnd);
}