#include "Scene_Game.h"
#include "st_SharedState.h"
#include "cl_GfxFactory.h"
#include <vector>
#include <cmath>
#include <fstream>
#include "ns_Vertex.h"

namespace {
  std::uniform_real_distribution<float> roidVecDist(-50, 50); //~~@ speed/angle dists

  std::vector<Vertex::Pos3Norm3Tex2> squareVerts = {
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, -1), DirectX::XMFLOAT2(0, 0) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(1, 0, 0), DirectX::XMFLOAT3(0, 0, -1), DirectX::XMFLOAT2(1, 0) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(0, 1, 0), DirectX::XMFLOAT3(0, 0, -1), DirectX::XMFLOAT2(0, 1) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(0, 1, 0), DirectX::XMFLOAT3(0, 0, -1), DirectX::XMFLOAT2(0, 1) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(1, 0, 0), DirectX::XMFLOAT3(0, 0, -1), DirectX::XMFLOAT2(1, 0) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(1, 1, 0), DirectX::XMFLOAT3(0, 0, -1), DirectX::XMFLOAT2(1, 1) },
  };

  constexpr float REGIONS_INF = 99999;
  const SC::Rect MID_REGION = { 50, 0, 750, 600 };
}

const Scene_Game::Regions Scene_Game::regions = {
  MID_REGION,
  // L                  T                   R                 B
  { -REGIONS_INF,      -REGIONS_INF,        MID_REGION.left, REGIONS_INF    }, //L
  { -REGIONS_INF,      -REGIONS_INF,        REGIONS_INF,     MID_REGION.top }, //T
  {  MID_REGION.right, -REGIONS_INF,        REGIONS_INF,     REGIONS_INF    }, //R
  { -REGIONS_INF,       MID_REGION.bottom,  REGIONS_INF,     REGIONS_INF    }  //B
};

Scene_Game::Scene_Game(SharedState& shared) : 
  Scene(shared),
  vShader(shared.factory.createVShader("../Assets/VertexShader.cso", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)),
  pShader(shared.factory.createPShader("../Assets/PixelShader.cso")), 
  tex(shared.factory.createTexture(L"../Assets/asteroid_diffuse.png")),
  cBuffer(shared.factory.createConstantBuffer<DirectX::XMFLOAT4X4>()),
  mesh(shared.factory.createStaticMeshFromOldMeshFileFormat("../Assets/asteroid.mesh")),
  roid({400,300}, { roidVecDist(shared.rng), roidVecDist(shared.rng) }, 50),
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

  paddle.xform.mulScale({ 20, 100, 1 });
  paddle.xform.translation.x = 765;
  paddle.xform.translation.y = ((float)shared.gfx.VIEWPORT_DIMS.height - paddle.xform.scale.y) / 2;
  paddle.collider.x(paddle.xform.translation.x - paddle.xform.scale.x);
  paddle.collider.width(paddle.xform.scale.x);
  paddle.collider.height(paddle.xform.scale.y);

  Transform bg;
  bg.translation.x = MID_REGION.left;
  bg.translation.z = 100;
  bg.scale.x = MID_REGION.width();
  bg.scale.y = MID_REGION.height();
  bgx = cam.getTransposedWVP(bg);
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
