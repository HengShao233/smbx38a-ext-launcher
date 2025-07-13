#include "../util/events/event.h"

#include "renderInit.h"
#include "renderInternalDef.h"

#include "drawable/quad.h"
#include "util/uuidGen.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler")

namespace ExEngine::Render
{
  static std::unique_ptr<Dx11Context> _ctx = nullptr;
  static std::unique_ptr<Dx11Resource> _res = nullptr;

  Dx11Resource& Res()
  {
    if (_res) return *_res;
    _res = std::make_unique<Dx11Resource>();
    return *_res;
  }

  Dx11Context& Ctx()
  {
    if (_ctx) return *_ctx;
    _ctx = std::make_unique<Dx11Context>();
    return *_ctx;
  }

  void DoInit()
  {
    XD::Event::StaticEventMgr::registerEvent<XD::Event::ExWndCreated>(XD::UUID::gen(), InitRdDevice);
  }

  static std::unique_ptr<Quad> _quad = nullptr;
  void Update(float t)
  {
    if (!_ctx) return;
    auto& ctx = *_ctx;
    if (!ctx.Inited) return;

    if (!_res) return;
    auto& res = *_res;
    if (!res.Inited) return;

    SendSmbxBuffer_Internal();
    auto& devCtx = *(ctx.DevCtx.Get());

    if (!_quad)
    {
      constexpr auto edgeWid = 0.04f;
      auto& wnd = ctx.TargetWnd;
      auto isHeight = (float)wnd.Height / wnd.Width < 4.0 / 3.0;
      auto refEdge = isHeight ? wnd.Height : wnd.Width;
      if (isHeight)
      {
        auto realEdgeWid = edgeWid * wnd.Height;
        auto realHeight = (wnd.Height - realEdgeWid * 2) / wnd.DPI;
        auto realWidth = realHeight / 3 * 4;
        _quad = std::make_unique<Quad>(DirectX::XMFLOAT4((wnd.Width - realWidth) / 2, realEdgeWid, realWidth, realHeight));
      }
      else
      {
        auto realEdgeWid = edgeWid * wnd.Width;
        auto realWidth = (wnd.Width - realEdgeWid * 2) / wnd.DPI;
        auto realHeight = realWidth / 4 * 3;
        _quad = std::make_unique<Quad>(DirectX::XMFLOAT4(realEdgeWid, (wnd.Height - realHeight) / 2, realWidth, realHeight));
      }
    }

    float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    devCtx.OMSetRenderTargets(1, res.RenderTargetView.GetAddressOf(), nullptr);
    devCtx.OMSetBlendState(res.BlendState.Get(), blendFactor, 0xFFFFFFFF);
    devCtx.OMSetDepthStencilState(res.DepthStencilState.Get(), 0);

    devCtx.RSSetViewports(1, &res.Viewport);
    devCtx.RSSetState(res.RasterizerState.Get());

    devCtx.IASetInputLayout(res.InputLayout.Get());
    devCtx.VSSetShader(res.BaseVertexShader->Get(), nullptr, 0);
    devCtx.PSSetShader(res.BasePixelShader->Get(), nullptr, 0);

    UINT offset = 0;
    UINT stride = sizeof(Vertex);
    devCtx.IASetVertexBuffers(0, 1, _quad->GetVertexBuffer().GetAddressOf(), &stride, &offset);
    devCtx.IASetIndexBuffer(_quad->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
    devCtx.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    devCtx.ClearRenderTargetView(res.RenderTargetView.Get(), clearColor);
    devCtx.VSSetConstantBuffers(0, 1, _quad->GetShapeBuffer().GetAddressOf());

    if (res.SmbxRenderTexView)
    {
      devCtx.PSSetShaderResources(0, 1, res.SmbxRenderTexView.GetAddressOf());
      devCtx.PSSetSamplers(0, 1, res.SmbxRenderDataSampler_Liner.GetAddressOf());
    }

    devCtx.DrawIndexed(6, 0, 0);
    ctx.Swapchain->Present(0, 0);
  }

  void DeInit()
  {
    if (!_ctx) return;
    { auto _ = _ctx.release(); }

    if (!_res) return;
    { auto _ = _res.release(); }
  }
}
