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

///////////ENTITIES///////////

GameScene::Entities::Entities(SharedState& shared, const Regions& regions, RenderProgram<DirectX::XMFLOAT4X4>& spriteProg, size_t numRoids) :
  asteroids(shared, regions, numRoids),
  paddles(shared, spriteProg),
  ball(shared, spriteProg, { 400, 300 }, Utility::randDirVec(shared.rng))
{
  //nop
}

void GameScene::Entities::update(float dt, const GameScene::Regions& regions) {
  asteroids.update(dt, regions);
  paddles.update(dt);
  ball.update(dt, *this, regions);
}

void GameScene::Entities::draw(Camera& cam) {
  asteroids.draw(cam);
  paddles.draw(cam);
  ball.draw(cam);
}

///////////BACKGROUND///////////

GameScene::BackGround::BackGround(SharedState& shared, RenderProgram<DirectX::XMFLOAT4X4>& spriteProg) :
  shared(shared),
  tex(shared.factory.createTexture(L"../Assets/black.png")),
  xform({ 0, 0, 900 }, { (float)shared.gfx.VIEWPORT_DIMS.width, (float)shared.gfx.VIEWPORT_DIMS.height, 1 }),
  spriteProg(&spriteProg),
  starProg{
    ShaderSet(
      shared.factory,
      "../Assets/vs_stars.cso", D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
      "", "", "",
      RasterizerState::DEFAULT_DESC, "../Assets/ps_stars.cso"
    ),
    shared.factory.createConstantBuffer<StarCB>()
  },
  starVerts(shared.factory.createStaticVertexBuffer(genStars(shared)))
{
  ZeroMemory(&starProg.cBuffer.object, sizeof(starProg.cBuffer.object));
}

void GameScene::BackGround::update(float dt) {
  starProg.cBuffer.object.offset.y += dt * 0.05f;
  if(starProg.cBuffer.object.offset.y > 2) { starProg.cBuffer.object.offset.y -= 2; }
  starProg.cBuffer.object.time += dt;
}

void GameScene::BackGround::draw(Camera& cam) {
  spriteProg->set();
  spriteProg->cBuffer.object = cam.getTransposedWVP(xform);
  spriteProg->cBuffer.update();
  tex.set(0);
  shared.gfx.draw(4);

  starProg.set();
  starVerts.set(0);
  starProg.cBuffer.update();
  shared.gfx.draw(starVerts.SIZE);
}

std::vector<GameScene::BackGround::StarVert> GameScene::BackGround::genStars(SharedState& shared) {
  auto& rng = shared.rng;

  static std::uniform_real_distribution<float> xDist(-1, 1);
  static std::uniform_real_distribution<float> yDist(-1, 1);
  static std::uniform_real_distribution<float> zDist(0.2f, 0.8f);

  static std::uniform_real_distribution<float> invPerDist(0.01f, 5);

  static std::uniform_real_distribution<float> redDist(0.5f, 1);
  static std::uniform_real_distribution<float> greenDist(0.8f, 1);
  static std::uniform_real_distribution<float> blueDist(0.6f, 1);

  std::vector<StarVert> sverts(NUM_STARS * 2);
  for(int i = 0; i < NUM_STARS; i++) {
    auto& star = sverts[i];
    float c = blueDist(rng);
    star.pos = { xDist(rng), yDist(rng), zDist(rng) };
    star.invPeriod = invPerDist(rng);
    star.col = { redDist(rng), greenDist(rng), blueDist(rng) };

    auto& dupe = sverts[i + NUM_STARS];
    dupe = star;
    dupe.pos.y -= 2;
  }

  return sverts;
}


