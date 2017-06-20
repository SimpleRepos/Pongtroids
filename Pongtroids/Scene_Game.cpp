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
  asteroids(shared, 3),
  paddles(shared, spriteProg),
  ball(shared, spriteProg, { 400, 300 }, Utility::randDirVec(shared.rng)),
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
  asteroids.update(dt);
  paddles.update(dt);
  ball.update(dt);

  ballVPaddles();
  ballVRoids();

  //~~@
  float ballX = ball.getCollider().center.x;
  if(ballX < 0)                                     { return new Scene_Game(shared); }
  if(ballX > (float)shared.gfx.VIEWPORT_DIMS.width) { return new Scene_Game(shared); }

  return this;
}

void Scene_Game::activeDraw() {
  shared.gfx.clear(ColorF::CYAN);
  bg.draw(cam);
  asteroids.draw(cam);
  paddles.draw(cam);
  ball.draw(cam);
}

void Scene_Game::ballVPaddles() {
  Paddles::Side side = (ball.getVelocity().x > 0) ? Paddles::RIGHT : Paddles::LEFT;

  auto& ballCol = ball.getCollider();

  if(SC::testOverlap(ballCol, paddles.getCollider(side))) {
    ball.setDirection(paddles.getDeflectionDirection(side, ballCol.center.y));
  }

}

void Scene_Game::ballVRoids() {
  auto& ballCol = ball.getCollider();
  for(auto& roid : asteroids.asteroids) {
    auto& roidCol = roid.getCollider();
    if(SC::testOverlap(ballCol, roidCol)) {
      ball.setDirection({ ballCol.center.x - roidCol.center.x, ballCol.center.y - roidCol.center.y });
      roid.hit({ ballCol.center.x, ballCol.center.y }, asteroids);
    }
  }
}
