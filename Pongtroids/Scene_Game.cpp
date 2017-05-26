#include "Scene_Game.h"
#include "st_SharedState.h"
#include "cl_GfxFactory.h"
#include <vector>
#include <cmath>
#include <fstream>
#include "ns_Vertex.h"

namespace {
  std::uniform_real_distribution<float> roidVecDist(-50, 50);

  std::vector<Vertex::Pos3Norm3Tex2> squareVerts = {
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(-1, +1, 0), DirectX::XMFLOAT3(0, 0, 1), DirectX::XMFLOAT2(0, 0) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(+1, +1, 0), DirectX::XMFLOAT3(0, 0, 1), DirectX::XMFLOAT2(1, 0) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(-1, -1, 0), DirectX::XMFLOAT3(0, 0, 1), DirectX::XMFLOAT2(0, 1) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(-1, -1, 0), DirectX::XMFLOAT3(0, 0, 1), DirectX::XMFLOAT2(0, 1) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(+1, +1, 0), DirectX::XMFLOAT3(0, 0, 1), DirectX::XMFLOAT2(1, 0) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(+1, -1, 0), DirectX::XMFLOAT3(0, 0, 1), DirectX::XMFLOAT2(1, 1) }
  };
}

Scene_Game::Scene_Game(SharedState& shared) : 
  Scene(shared),
  vShader(shared.factory.createVShader("../Assets/VertexShader.cso", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)),
  pShader(shared.factory.createPShader("../Assets/PixelShader.cso")), 
  tex(shared.factory.createTexture(L"../Assets/asteroid_diffuse.png")),
  cBuffer(shared.factory.createConstantBuffer<DirectX::XMFLOAT4X4>()),
  mesh(shared.factory.createStaticMeshFromOldMeshFileFormat("../Assets/asteroid.mesh")),
  roid({0,0}, { roidVecDist(shared.rng), roidVecDist(shared.rng) }),
  paddleMesh(shared.factory.createStaticMeshFromVertices(squareVerts)),
  black(shared.factory.createTexture(L"../Assets/black.png"))
{
  shared.win.addKeyFunc(VK_ESCAPE, [](HWND, LPARAM) { PostQuitMessage(0); });

  vShader.set();
  pShader.set();
  tex.set(0);
  cBuffer.set(0);

  cam.setOrthographic((float)shared.gfx.VIEWPORT_DIMS.width, (float)shared.gfx.VIEWPORT_DIMS.height);
  cam.setDepthLimits(-100, 1000);
  cam.setEyePos(0, 0, -5);
  cam.setTargetDir(0, 0, 1);

  paddle.xform.mulScale({ 10, 50, 1 });
  paddle.xform.translation.x = 375;
  paddle.collider.x(paddle.xform.translation.x - paddle.xform.scale.x);
  paddle.collider.width(paddle.xform.scale.x * 2);
  paddle.collider.height(paddle.xform.scale.y * 2);

  bgx = cam.getTransposedWVP(Transform{ {0,0,100}, {0,0,0,1}, {Asteroid::bounds.right, Asteroid::bounds.top, 1} });
}

Scene* Scene_Game::activeUpdate() {
  float dt = (float)shared.timer.getTickDT();
  roid.update(dt);
  paddle.update(shared, dt);
  return this;
}

void Scene_Game::activeDraw() {
  shared.gfx.clear(ColorF::CYAN);

  tex.set(0);
  cBuffer.object = cam.getTransposedWVP(roid.xform);
  cBuffer.update();
  shared.gfx.draw(mesh);

  cBuffer.object = cam.getTransposedWVP(paddle.xform);
  cBuffer.update();
  shared.gfx.draw(paddleMesh);

  black.set(0);
  cBuffer.object = bgx;
  cBuffer.update();
  shared.gfx.draw(paddleMesh);

}
