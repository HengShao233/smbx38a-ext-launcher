#pragma once

#include <Windows.h>
#include "../smbx/smbxContext.h"

namespace ExEngine::Render
{
	void InitRdDevice(SMBX::GameWnd);	// 初始化渲染设备
	void InitRdResource();				// 初始化渲染资源
}