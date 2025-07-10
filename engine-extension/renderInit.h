#pragma once

#include <Windows.h>
#include "smbxContext.h"

namespace ExEngine::Render
{
	void InitRdDevice(SMBX::GameWnd);	// 初始化渲染设备
	void InitRdResource();				// 初始化渲染资源
}