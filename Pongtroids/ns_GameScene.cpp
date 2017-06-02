#include "ns_GameScene.h"

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

GameScene::RenderProgram::RenderProgram(SharedState& shared) :
  shaders(
    shared.factory,
    "../Assets/VertexShader.cso", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, "", "", "",
    RasterizerState::DEFAULT_DESC, "../Assets/PixelShader.cso"
  ),
  cam(),
  cBuffer(shared.factory.createConstantBuffer<DirectX::XMFLOAT4X4>()),
  squareMesh(shared.factory.createStaticMeshFromVertices(squareVerts))
{
  cam.setOrthographic((float)shared.gfx.VIEWPORT_DIMS.width, (float)shared.gfx.VIEWPORT_DIMS.height);
  cam.setDepthLimits(-100, 1000);
  cam.setEyePos(0, 0, -5);
  cam.setTargetDir(0, 0, 1);
}

void GameScene::RenderProgram::set() {
  shaders.set();
  cBuffer.set(0);
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

void GameScene::Entities::draw(RenderProgram& prog) {
  asteroids.draw(prog);
  paddles.draw(prog);
  ball.draw(prog);
}

///////////BACKGROUND///////////

GameScene::BackGround::BackGround(SharedState& shared, RenderProgram& prog, const Regions& regions) :
  shared(shared),
  tex(shared.factory.createTexture(L"../Assets/black.png"))
{
  Transform bg({ regions.middle.left, 0, 100 }, { regions.middle.width(), regions.middle.height(), 1 });
  bgx = prog.cam.getTransposedWVP(bg);
}

void GameScene::BackGround::draw(RenderProgram& prog) {
  tex.set(0);
  prog.cBuffer.object = bgx;
  prog.cBuffer.update();
  shared.gfx.draw(prog.squareMesh);
}

