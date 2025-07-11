#pragma once

#include <DirectXMath.h>

namespace ExEngine::Render
{
	struct Vertex
	{
		DirectX::XMFLOAT3 Pos = {};
		DirectX::XMFLOAT2 UV = {};
	};
}
