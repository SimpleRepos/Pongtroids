#include "Scene_Game.h"
#include "st_SharedState.h"
#include "cl_GfxFactory.h"
#include <vector>
#include <cmath>
#include <fstream>
#include "ns_Vertex.h"
#include "cl_Window.h"
#include "ns_Utility.h"

namespace {
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
  colliderSet{ regions, lPaddle.collider, rPaddle.collider, asteroids },
  shaders(
    shared.factory,
    "../Assets/VertexShader.cso", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, "", "", "",
    RasterizerState::DEFAULT_DESC, "../Assets/PixelShader.cso"
  ),
  cam(),
  cBuffer(shared.factory.createConstantBuffer<DirectX::XMFLOAT4X4>()),
  asteroids(shared, regions, 3), //~~@ magic
  rPaddle(765, 250, 20, 100, regions.middle),
  lPaddle( 15, 250, 20, 100, regions.middle),
  squareMesh(shared.factory.createStaticMeshFromVertices(squareVerts)),
  black(shared.factory.createTexture(L"../Assets/black.png")),
  white(shared.factory.createTexture(L"../Assets/white.png")),
  ball({ 200, 200 }, Utility::randDirVec(shared.rng))
{
  shared.win.addKeyFunc(VK_ESCAPE, [](HWND, LPARAM) { PostQuitMessage(0); });

  shaders.set();
  cBuffer.set(0);

  cam.setOrthographic((float)shared.gfx.VIEWPORT_DIMS.width, (float)shared.gfx.VIEWPORT_DIMS.height);
  cam.setDepthLimits(-100, 1000);
  cam.setEyePos(0, 0, -5);
  cam.setTargetDir(0, 0, 1);

  Transform bg = { { regions.middle.left, 0, 100 }, {0,0,0,1}, { regions.middle.width(), regions.middle.height(), 1 } };
  bgx = cam.getTransposedWVP(bg);
}

Scene* Scene_Game::activeUpdate() {
  float dt = (float)shared.timer.getTickDT();
  asteroids.update(dt);
  rPaddle.update(dt, shared);
  lPaddle.update(rPaddle);
  ball.update(dt, colliderSet);
  return this;
}

void Scene_Game::activeDraw() {
  shared.gfx.clear(ColorF::CYAN);

  asteroids.draw(cBuffer, cam);

  white.set(0);

  cBuffer.object = cam.getTransposedWVP(rPaddle.xform);
  cBuffer.update();
  shared.gfx.draw(squareMesh);

  cBuffer.object = cam.getTransposedWVP(lPaddle.xform);
  cBuffer.update();
  shared.gfx.draw(squareMesh);

  cBuffer.object = cam.getTransposedWVP(ball.xform);
  cBuffer.update();
  shared.gfx.draw(squareMesh);

  black.set(0);
  cBuffer.object = bgx;
  cBuffer.update();
  shared.gfx.draw(squareMesh);
}
