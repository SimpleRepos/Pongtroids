#include "ns_GameScene.h"
#include "cl_Camera.h"

///////////REGIONS///////////

GameScene::Regions::Regions(SharedState& shared) {
  const float WIDE = (float)shared.gfx.VIEWPORT_DIMS.width;
  const float TALL = (float)shared.gfx.VIEWPORT_DIMS.height;
  const float LBORDER = 50;
  const float RBORDER = WIDE - LBORDER;

             //L        T     R        B
    middle = { LBORDER, 0,    RBORDER, TALL };
    left   = { 0,       0,    LBORDER, TALL };
    top    = { 0,       0,    WIDE,    0    };
    right  = { RBORDER, 0,    WIDE,    TALL };
    bottom = { 0,       TALL, WIDE,    TALL };
}

///////////RENDER PROGRAM///////////

namespace {
  const std::vector<Vertex::Pos3Norm3Tex2> squareVerts = {
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, -1), DirectX::XMFLOAT2(0, 0) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(1, 0, 0), DirectX::XMFLOAT3(0, 0, -1), DirectX::XMFLOAT2(1, 0) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(0, 1, 0), DirectX::XMFLOAT3(0, 0, -1), DirectX::XMFLOAT2(0, 1) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(0, 1, 0), DirectX::XMFLOAT3(0, 0, -1), DirectX::XMFLOAT2(0, 1) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(1, 0, 0), DirectX::XMFLOAT3(0, 0, -1), DirectX::XMFLOAT2(1, 0) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(1, 1, 0), DirectX::XMFLOAT3(0, 0, -1), DirectX::XMFLOAT2(1, 1) }
  };
}

StaticMesh GameScene::genSquareMesh(SharedState& shared) {
  return shared.factory.createStaticMeshFromVertices(squareVerts);
}

///////////ENTITIES///////////

GameScene::Entities::Entities(SharedState& shared, const Regions& regions, size_t numRoids) :
  asteroids(shared, regions, numRoids),
  paddles(shared),
  ball(shared, { 400, 300 }, Utility::randDirVec(shared.rng))
{
  //nop
}

void GameScene::Entities::update(float dt, const GameScene::Regions& regions) {
  asteroids.update(dt, regions);
  paddles.update(dt);
  ball.update(dt, *this, regions);
}

void GameScene::Entities::draw(RenderProgram<DirectX::XMFLOAT4X4>& prog, Camera& cam) {
  asteroids.draw(prog, cam);
  paddles.draw(prog, cam);
  ball.draw(prog, cam);
}

///////////BACKGROUND///////////

GameScene::BackGround::BackGround(SharedState& shared, const Regions& regions) :
  shared(shared),
  tex(shared.factory.createTexture(L"../Assets/black.png")),
  mesh(shared.factory.createStaticMeshFromVertices(squareVerts)),
  xform({ regions.middle.left, 0, 100 }, { regions.middle.width(), regions.middle.height(), 1 })
{
  //nop
}

void GameScene::BackGround::draw(RenderProgram<DirectX::XMFLOAT4X4>& prog, Camera& cam) {
  prog.set();
  prog.cBuffer.object = cam.getTransposedWVP(xform);
  prog.cBuffer.update();
  tex.set(0);
  shared.gfx.draw(mesh);
}

