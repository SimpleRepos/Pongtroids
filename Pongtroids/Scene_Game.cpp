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
  renderProg{
    ShaderSet(
      shared.factory,
      "../Assets/vs_temporarySimpleTransform.cso", D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
      "", "", "",
      RasterizerState::DEFAULT_DESC, "../Assets/ps_texture.cso"
    ),
    shared.factory.createConstantBuffer<DirectX::XMFLOAT4X4>()
  },
  cam(),
  squareMesh(GameScene::genSquareMesh(shared)),
  entities(shared, regions, 3),
  bg(shared, regions)
{
  shared.win.addKeyFunc(VK_ESCAPE, [](HWND, LPARAM) { PostQuitMessage(0); });
  renderProg.set();

  cam.setOrthographic((float)shared.gfx.VIEWPORT_DIMS.width, (float)shared.gfx.VIEWPORT_DIMS.height);
  cam.setDepthLimits(-100, 1000);
  cam.setEyePos(0, 0, -5);
  cam.setTargetDir(0, 0, 1);
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
  bg.draw(renderProg, cam);
  entities.draw(renderProg, cam);
}

