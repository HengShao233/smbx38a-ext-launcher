#pragma once

#include "vertex.h"
#include <DirectXMath.h>

#include "renderInternalDef.h"

namespace ExEngine::Render
{
	class Quad
	{
	public:
		struct alignas(16) ShapeBuffer
		{
			DirectX::XMFLOAT4 Shape;
			DirectX::XMFLOAT4 Color;
			float Depth;
		};

	public:
		Quad();
		Quad(const DirectX::XMFLOAT4& rect, float depth = 0.001f);

		void SetDepth(float depth);
		void SetColor(const DirectX::XMFLOAT4& col);
		void SetShape(const DirectX::XMFLOAT4& shape);

		ComPtr<ID3D11Buffer>& GetShapeBuffer();
		static ComPtr<ID3D11Buffer>& GetIndexBuffer();
		static ComPtr<ID3D11Buffer>& GetVertexBuffer();

	private:
		bool _dirty = true;
		bool _bufferInited = false;
		DirectX::XMFLOAT4 _rect = {};
		ShapeBuffer _shape = {};

		ComPtr<ID3D11Buffer> _shapeBuffer = nullptr;

		void Refresh();

		static Vertex _vert[4];
		static UINT _indices[6];
		static ComPtr<ID3D11Buffer> _vertBuffer;
		static ComPtr<ID3D11Buffer> _indexBuffer;
		static ComPtr<ID3D11Buffer> _empty;
	};
}
