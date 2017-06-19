#include "cl_GameBackground.h"
#include "cl_Camera.h"

GameBackGround::GameBackGround(SharedState& shared, RenderProgram<DirectX::XMFLOAT4X4>& spriteProg) :
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

void GameBackGround::update(float dt) {
  starProg.cBuffer.object.offset.y += dt * 0.05f;
  if(starProg.cBuffer.object.offset.y > 2) { starProg.cBuffer.object.offset.y -= 2; }
  starProg.cBuffer.object.time += dt;
}

void GameBackGround::draw(Camera& cam) {
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

std::vector<GameBackGround::StarVert> GameBackGround::genStars(SharedState& shared) {
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


