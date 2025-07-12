#pragma once

#include <Windows.h>

#include "smbxContext.h"
#include "util/staticEventMgr.hpp"

namespace XD::Event
{
  class WndLoaded : public EventTypeBase<WndLoaded, HWND, HINSTANCE, int> {};
  class ExWndCreated : public EventTypeBase<ExWndCreated, ExEngine::SMBX::GameWnd> {};
}
