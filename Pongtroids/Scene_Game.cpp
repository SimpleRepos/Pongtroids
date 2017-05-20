#include "Scene_Game.h"
#include "st_SharedState.h"
#include "cl_GfxFactory.h"
#include <vector>
#include "ns_Vertex.h"

namespace {
  const std::vector<Vertex::Pos4Nrm4Tex2> triVerts = {
    { DirectX::XMFLOAT4( 0.0f,  0.5f, 0, 1), DirectX::XMFLOAT4(0, 0, -1, 0), DirectX::XMFLOAT2(0.5f, 0.0f) },
    { DirectX::XMFLOAT4( 0.5f, -0.5f, 0, 1), DirectX::XMFLOAT4(0, 0, -1, 0), DirectX::XMFLOAT2(1.0f, 1.0f) },
    { DirectX::XMFLOAT4(-0.5f, -0.5f, 0, 1), DirectX::XMFLOAT4(0, 0, -1, 0), DirectX::XMFLOAT2(0.0f, 1.0f) }
  };
}

Scene_Game::Scene_Game(SharedState& shared) : 
  Scene(shared),
  vShader(shared.factory.createVShader("../Debug/VertexShader.cso", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)),
  pShader(shared.factory.createPShader("../Debug/PixelShader.cso")),
  verts(shared.factory.createStaticVertexBuffer(triVerts)),
  tex(shared.factory.createTexture(L"../Assets/checkers.png")),
  cBuffer(shared.factory.createConstantBuffer<CB>())
{
  shared.win.addKeyFunc(VK_ESCAPE, [](HWND, LPARAM) { PostQuitMessage(0); });

  vShader.set();
  pShader.set();
  verts.set(0);
  tex.set(0);
  cBuffer.set(0);

  cam.setAspectRatio((float)shared.gfx.VIEWPORT_DIMS.width, (float)shared.gfx.VIEWPORT_DIMS.height);
  cam.setEyePos(0, 0, -5);
  cam.setTargetDir(0, 0, 1);

  xform.scale.y = 2;

}

Scene* Scene_Game::update() {
  xform.rotate(DirectX::XMFLOAT3(0, 0, 1), (float)shared.timer.getTickDT());
  return this;
}

void Scene_Game::draw() {
  shared.gfx.clear(ColorF::MAGENTA);

  cBuffer.object.transWVP = cam.getTransposedWVP(xform.asMatrix());
  cBuffer.update();
  shared.gfx.draw(verts.SIZE);

}
