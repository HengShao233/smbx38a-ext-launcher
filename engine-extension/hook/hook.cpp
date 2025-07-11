#include "../hook/hook.h"
#pragma comment(lib, "../detours/lib.X86/detours.lib")

#include "../hook/d3d9/d3dHook.h"
#include "msgBoxHook.h"
#include "showWindowHook.h"
#include "getMessageHook.h"

namespace ExEngine::Hook
{
	void Attach()
	{
		AttachD3dDetours();
		AttachMsgBoxDetours();
		AttachShowWindowDetours();
		AttachGetMessageDetours();
	}

	void Detach()
	{
		DetachD3dDetours();
		DetachMsgBoxDetours();
		DetachShowWindowDetours();
		DetachGetMessageDetours();
	}
}