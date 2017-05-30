#include "Scene_Game.h"
#include "st_SharedState.h"
#include "cl_GfxFactory.h"
#include <vector>
#include <cmath>
#include <fstream>
#include "ns_Vertex.h"
#include "cl_Window.h"

namespace {
  DirectX::XMFLOAT2 randomDirectionScaledVector(std::mt19937& rng) { //~~@ move this to util
    static std::uniform_real_distribution<float> radianDist(0, DirectX::XM_2PI);
    float radians = radianDist(rng);
    return { std::cosf(radians), std::sinf(radians) };
  }

  std::vector<Vertex::Pos3Norm3Tex2> squareVerts = {
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, -1), DirectX::XMFLOAT2(0, 0) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(1, 0, 0), DirectX::XMFLOAT3(0, 0, -1), DirectX::XMFLOAT2(1, 0) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(0, 1, 0), DirectX::XMFLOAT3(0, 0, -1), DirectX::XMFLOAT2(0, 1) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(0, 1, 0), DirectX::XMFLOAT3(0, 0, -1), DirectX::XMFLOAT2(0, 1) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(1, 0, 0), DirectX::XMFLOAT3(0, 0, -1), DirectX::XMFLOAT2(1, 0) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(1, 1, 0), DirectX::XMFLOAT3(0, 0, -1), DirectX::XMFLOAT2(1, 1) },
  };


  Regions genRegions(const Window::Dimensions& VIEWPORT_DIMS) {
    const float WIDE = (float)VIEWPORT_DIMS.width;
    const float TALL = (float)VIEWPORT_DIMS.height;
    constexpr float LBORDER = 50;
    const float RBORDER = WIDE - LBORDER;

    return {
      //L        T     R        B
      { LBORDER, 0,    RBORDER, TALL }, //MID
      { 0,       0,    LBORDER, TALL }, //L
      { 0,       0,    WIDE,    0    }, //T
      { RBORDER, 0,    WIDE,    TALL }, //R
      { 0,       TALL, WIDE,    TALL }  //B
    };
  }
}

Scene_Game::Scene_Game(SharedState& shared) : 
  Scene(shared),
  regions(genRegions(shared.gfx.VIEWPORT_DIMS)),
  colliderSet{ regions, lPaddle.collider, rPaddle.collider, {} },
  vShader(shared.factory.createVShader("../Assets/VertexShader.cso", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)),
  pShader(shared.factory.createPShader("../Assets/PixelShader.cso")), 
  tex(shared.factory.createTexture(L"../Assets/asteroid_diffuse.png")),
  cBuffer(shared.factory.createConstantBuffer<DirectX::XMFLOAT4X4>()),
  mesh(shared.factory.createStaticMeshFromOldMeshFileFormat("../Assets/asteroid.mesh")),
  roid({400,300}, randomDirectionScaledVector(shared.rng), 50, regions),
  rPaddle(765, 250, 20, 100, regions.middle),
  lPaddle( 15, 250, 20, 100, regions.middle),
  paddleMesh(shared.factory.createStaticMeshFromVertices(squareVerts)),
  black(shared.factory.createTexture(L"../Assets/black.png")),
  ball({ 200, 200 }, randomDirectionScaledVector(shared.rng))
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

  Transform bg = { { regions.middle.left, 0, 100 }, {0,0,0,1}, { regions.middle.width(), regions.middle.height(), 1 } };
  bgx = cam.getTransposedWVP(bg);

  colliderSet.asteroids.push_back(&roid);
}

Scene* Scene_Game::activeUpdate() {
  float dt = (float)shared.timer.getTickDT();
  roid.update(dt);
  rPaddle.update(dt, shared);
  lPaddle.update(rPaddle);
  ball.update(dt, colliderSet);
  return this;
}

void Scene_Game::activeDraw() {
  shared.gfx.clear(ColorF::CYAN);

  tex.set(0);
  cBuffer.object = cam.getTransposedWVP(roid.xform);
  cBuffer.update();
  shared.gfx.draw(mesh);

  cBuffer.object = cam.getTransposedWVP(rPaddle.xform);
  cBuffer.update();
  shared.gfx.draw(paddleMesh);

  cBuffer.object = cam.getTransposedWVP(lPaddle.xform);
  cBuffer.update();
  shared.gfx.draw(paddleMesh);

  cBuffer.object = cam.getTransposedWVP(ball.xform);
  cBuffer.update();
  shared.gfx.draw(paddleMesh);

  black.set(0);
  cBuffer.object = bgx;
  cBuffer.update();
  shared.gfx.draw(paddleMesh);
}
