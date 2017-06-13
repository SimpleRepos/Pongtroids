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
  cam(),
  entities(shared, regions, spriteProg, 3),
  bg(shared, spriteProg)
{
  D3D11_RENDER_TARGET_BLEND_DESC desc = {
    TRUE, //enable
    D3D11_BLEND_SRC_ALPHA, //src
    D3D11_BLEND_INV_SRC_ALPHA, //dest
    D3D11_BLEND_OP_ADD, //op
    D3D11_BLEND_ONE, //srca
    D3D11_BLEND_ONE, //desta
    D3D11_BLEND_OP_ADD, //opa
    D3D11_COLOR_WRITE_ENABLE_ALL //write mask
  };
  shared.gfx.setBlend(desc);

  shared.win.addKeyFunc(VK_ESCAPE, [](HWND, LPARAM) { PostQuitMessage(0); });

  cam.setOrthographic((float)shared.gfx.VIEWPORT_DIMS.width, (float)shared.gfx.VIEWPORT_DIMS.height);
  cam.setDepthLimits(-100, 1000);
  cam.setEyePos(0, 0, -5);
  cam.setTargetDir(0, 0, 1);
}

Scene* Scene_Game::activeUpdate() {
  constexpr float MAX_FRAME_LENGTH = 1.0f / 60;
  float dt = Utility::clamp((float)shared.timer.getTickDT(), 0, MAX_FRAME_LENGTH);

  bg.update(dt);
  entities.update(dt, regions);
  if(entities.ball.xform.translation.x < regions.left.left)   { return new Scene_Game(shared); }
  if(entities.ball.xform.translation.x > regions.right.right) { return new Scene_Game(shared); }
  return this;
}

void Scene_Game::activeDraw() {
  shared.gfx.clear(ColorF::CYAN);
  bg.draw(cam);
  entities.draw(cam);
}

