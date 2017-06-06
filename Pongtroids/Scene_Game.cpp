#include "Scene_Game.h"
#include "st_SharedState.h"
#include "cl_GfxFactory.h"
#include <vector>
#include <cmath>
#include <fstream>
#include "ns_Vertex.h"
#include "cl_Window.h"
#include "ns_Utility.h"


Scene_Game::Scene_Game(SharedState& shared) : 
  Scene(shared),
  regions(shared),

  spriteProg{
    ShaderSet(
      shared.factory,
      "../Assets/vs_sprite.cso", D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
      "", "", "",
      RasterizerState::DEFAULT_DESC, "../Assets/ps_texture.cso"
    ),
    shared.factory.createConstantBuffer<DirectX::XMFLOAT4X4>()
  },
  roidProg{
    ShaderSet(
      shared.factory,
      "../Assets/vs_temporarySimpleTransform.cso", D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
      "", "", "",
      RasterizerState::DEFAULT_DESC, "../Assets/ps_texture.cso"
    ),
    shared.factory.createConstantBuffer<DirectX::XMFLOAT4X4>()
  },
  starProg{
    ShaderSet(
      shared.factory,
      "../Assets/vs_stars.cso", D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
      "", "", "",
      RasterizerState::DEFAULT_DESC, "../Assets/ps_stars.cso"
    ),
    shared.factory.createConstantBuffer<StarCB>()
  },
  cam(),
  starVerts(shared.factory.createStaticVertexBuffer(genStars(shared))),
  entities(shared, regions, 3),
  bg(shared, regions)
{
  shared.win.addKeyFunc(VK_ESCAPE, [](HWND, LPARAM) { PostQuitMessage(0); });
  roidProg.set();

  cam.setOrthographic((float)shared.gfx.VIEWPORT_DIMS.width, (float)shared.gfx.VIEWPORT_DIMS.height);
  cam.setDepthLimits(-100, 1000);
  cam.setEyePos(0, 0, -5);
  cam.setTargetDir(0, 0, 1);

  starProg.cBuffer.object.offset = DirectX::XMFLOAT3();
}

Scene* Scene_Game::activeUpdate() {
  constexpr float MAX_FRAME_LENGTH = 1.0f / 60;
  float dt = Utility::clamp((float)shared.timer.getTickDT(), 0, MAX_FRAME_LENGTH);
  entities.update(dt, regions);
  if(entities.ball.xform.translation.x < regions.left.left)   { return new Scene_Game(shared); }
  if(entities.ball.xform.translation.x > regions.right.right) { return new Scene_Game(shared); }
  return this;
}

void Scene_Game::activeDraw() {
  shared.gfx.clear(ColorF::CYAN);
  bg.draw(spriteProg, cam);

  starProg.set();
  starVerts.set(0);
  starProg.cBuffer.object.offset.y += (float)shared.timer.getTickDT() * 0.05f;
  if(starProg.cBuffer.object.offset.y > 2) { starProg.cBuffer.object.offset.y -= 2; }
  starProg.cBuffer.object.time = (float)shared.timer.sinceStart();
  starProg.cBuffer.update();
  shared.gfx.draw(starVerts.SIZE);

  entities.draw(roidProg, spriteProg, cam);
}

std::vector<Scene_Game::StarVert> Scene_Game::genStars(SharedState& shared) {
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


