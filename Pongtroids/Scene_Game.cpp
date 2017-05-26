#include "Scene_Game.h"
#include "st_SharedState.h"
#include "cl_GfxFactory.h"
#include <vector>
#include <cmath>
#include <fstream>
#include "ns_Vertex.h"
#include <random>

namespace {

  std::vector<Vertex::Pos3Norm3Tex2> paddleVerts = {
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(-0.125f, +0.5f, 0), DirectX::XMFLOAT3(0, 0, 1), DirectX::XMFLOAT2(0, 0) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(+0.125f, +0.5f, 0), DirectX::XMFLOAT3(0, 0, 1), DirectX::XMFLOAT2(1, 0) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(-0.125f, -0.5f, 0), DirectX::XMFLOAT3(0, 0, 1), DirectX::XMFLOAT2(0, 1) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(-0.125f, -0.5f, 0), DirectX::XMFLOAT3(0, 0, 1), DirectX::XMFLOAT2(0, 1) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(+0.125f, +0.5f, 0), DirectX::XMFLOAT3(0, 0, 1), DirectX::XMFLOAT2(1, 0) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(+0.125f, -0.5f, 0), DirectX::XMFLOAT3(0, 0, 1), DirectX::XMFLOAT2(1, 1) }
  };
}

Scene_Game::Scene_Game(SharedState& shared) : 
  Scene(shared),
  vShader(shared.factory.createVShader("../Assets/VertexShader.cso", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)),
  pShader(shared.factory.createPShader("../Assets/PixelShader.cso")), 
  tex(shared.factory.createTexture(L"../Assets/asteroid_diffuse.png")),
  cBuffer(shared.factory.createConstantBuffer<DirectX::XMFLOAT4X4>()),
  mesh(shared.factory.createStaticMeshFromOldMeshFileFormat("../Assets/asteroid.mesh")),
  roid({0,0}, {0.2f, 0.3f}),
  paddleMesh(shared.factory.createStaticMeshFromVertices(paddleVerts))
{
  shared.win.addKeyFunc(VK_ESCAPE, [](HWND, LPARAM) { PostQuitMessage(0); });

  vShader.set();
  pShader.set();
  tex.set(0);
  cBuffer.set(0);

  cam.setAspectRatio((float)shared.gfx.VIEWPORT_DIMS.width, (float)shared.gfx.VIEWPORT_DIMS.height);
  cam.setEyePos(0, 0, -5);
  cam.setTargetDir(0, 0, 1);

  auto& ppos = paddle.xform.translation;
  ppos.x = paddle.RIGHT_X;

}

Scene* Scene_Game::activeUpdate() {
  float dt = (float)shared.timer.getTickDT();
  roid.update(dt);
  paddle.update(shared, dt);
  return this;
}

void Scene_Game::activeDraw() {
  shared.gfx.clear(ColorF::MAGENTA);

  cBuffer.object = cam.getTransposedWVP(roid.xform);
  cBuffer.update();
  shared.gfx.draw(mesh);

  cBuffer.object = cam.getTransposedWVP(paddle.xform);
  cBuffer.update();
  shared.gfx.draw(paddleMesh);

}
