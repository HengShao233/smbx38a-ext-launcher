#include "quad.h"
#include "util/logger.h"

namespace ExEngine::Render
{
  Quad::Quad()
  {
    _shape.Depth = 0.001f;
    _shape.Color = DirectX::XMFLOAT4(1, 1, 1, 1);
  }

  Quad::Quad(const DirectX::XMFLOAT4& rect, float depth)
  {
    _rect = rect;
    _shape.Depth = depth;
    _shape.Color = DirectX::XMFLOAT4(1, 1, 1, 1);
  }

  void Quad::SetDepth(float depth)
  {
    _dirty = true;
    _shape.Depth = depth;
  }

  void Quad::SetColor(const DirectX::XMFLOAT4& col)
  {
    _dirty = true;
    _shape.Color = col;
  }

  void Quad::SetShape(const DirectX::XMFLOAT4& shape)
  {
    _dirty = true;
    _rect = shape;
  }

  ComPtr<ID3D11Buffer>& Quad::GetShapeBuffer()
  {
    auto& ctx = Ctx();
    if (!ctx.Inited) return _empty;
    if (_shapeBuffer)
    {
      Refresh();
      return _shapeBuffer;
    }

    _shapeBuffer = nullptr;
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = sizeof(ShapeBuffer);
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    HRESULT hr = ctx.Dev->CreateBuffer(&bufferDesc, nullptr, _shapeBuffer.GetAddressOf());
    if (FAILED(hr)) {
      Logger::Error("failed to create constant buffer");
      return _empty;
    }
    _bufferInited = false;

    Refresh();
    return _shapeBuffer;
  }

  ComPtr<ID3D11Buffer>& Quad::GetIndexBuffer()
  {
    auto& ctx = Ctx();
    if (!ctx.Inited) return _empty;
    if (_indexBuffer) return _indexBuffer;

    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(_indices);
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = _indices;
    ctx.Dev->CreateBuffer(&indexBufferDesc, &indexData, _indexBuffer.GetAddressOf());
    return _indexBuffer;
  }

  void Quad::Refresh()
  {
    auto& ctx = Ctx();
    if (!ctx.Inited) return;
    if (!_shapeBuffer) return;
    if (!_dirty) return;

    _shape.Shape = DirectX::XMFLOAT4(
      _rect.x / ctx.TargetWnd.Width,
      _rect.y / ctx.TargetWnd.Height,
      _rect.z / ctx.TargetWnd.Width,
      _rect.w / ctx.TargetWnd.Height
    );

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    auto hr = ctx.DevCtx->Map(_shapeBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(hr)) {
      Logger::Error("failed to map constant buffer");
      return;
    }

    memcpy(mappedResource.pData, &_shape, sizeof(ShapeBuffer));
    ctx.DevCtx->Unmap(_shapeBuffer.Get(), 0);
    _bufferInited = true;
  }

  ComPtr<ID3D11Buffer>& Quad::GetVertexBuffer()
  {
    auto &ctx = Ctx();
    if (!ctx.Inited) return _empty;
    if (_vertBuffer) return _vertBuffer;

    _vertBuffer = nullptr;
    D3D11_BUFFER_DESC bufferDesc = { sizeof(_vert), D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER };
    D3D11_SUBRESOURCE_DATA initData = { _vert };

    ctx.Dev->CreateBuffer(&bufferDesc, &initData, _vertBuffer.GetAddressOf());
    return _vertBuffer;
  }

  Vertex Quad::_vert[4] = {
    { DirectX::XMFLOAT3(-1.0f, 1.0f, 0.001f), DirectX::XMFLOAT2(0.0f, 0.0f) },
    { DirectX::XMFLOAT3(1.0f, 1.0f, 0.001f), DirectX::XMFLOAT2(1.0f, 0.0f) },
    { DirectX::XMFLOAT3(-1.0f, -1.0f, 0.001f), DirectX::XMFLOAT2(0.0f, 1.0f) },
    { DirectX::XMFLOAT3(1.0f, -1.0f, 0.001f), DirectX::XMFLOAT2(1.0f, 1.0f) },
  };

  UINT Quad::_indices[6] = {
    0, 1, 2,
    2, 1, 3
  };

  ComPtr<ID3D11Buffer> Quad::_vertBuffer = nullptr;
  ComPtr<ID3D11Buffer> Quad::_indexBuffer = nullptr;
  ComPtr < ID3D11Buffer> Quad::_empty = nullptr;
}

