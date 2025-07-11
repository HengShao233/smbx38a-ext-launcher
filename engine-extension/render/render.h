#pragma once

#include <d3d9.h>

namespace ExEngine::Render
{
	void DoInit();
	void Update(float t);
	void DeInit();

	void SendSmbxBuffer(IDirect3DSurface9* backBuffer, IDirect3DDevice9* dev);	// 提呈 smbx38a 的渲染结果到外挂引擎
}