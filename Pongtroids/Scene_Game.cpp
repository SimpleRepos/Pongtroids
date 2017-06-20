#include "Scene_Game.h"
#include "st_SharedState.h"
#include "cl_GfxFactory.h"
#include <vector>
#include <cmath>
#include <fstream>
#include "ns_Vertex.h"
#include "cl_Window.h"
#include "ns_Utility.h"
#include "Scene_Null.h"

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
  bg(shared, spriteProg),
  asteroids(shared, 3),
  paddles(shared, spriteProg),
  ball(shared, spriteProg, { 400, 300 }, Utility::randDirVec(shared.rng)),
  LEFT_OOB(0),
  RIGHT_OOB((float)shared.gfx.VIEWPORT_DIMS.width),
  reset(false)
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

  cam.setOrthographic((float)shared.gfx.VIEWPORT_DIMS.width, (float)shared.gfx.VIEWPORT_DIMS.height);
  cam.setDepthLimits(-100, 1000);
  cam.setEyePos(0, 0, -5);
  cam.setTargetDir(0, 0, 1);
}

void Scene_Game::passiveUpdate() {
  float dt = (float)shared.timer.getTickDT();
  bg.update(dt);
}

Scene* Scene_Game::activeUpdate() {
  float dt = (float)shared.timer.getTickDT();

  asteroids.update(dt);
  paddles.update(dt);
  ball.update(dt);

  ballVPaddles();
  ballVRoids();

  return processWinLoss();
}

void Scene_Game::passiveDraw() {
  shared.gfx.clear(ColorF::CYAN);
  bg.draw(cam);
  asteroids.draw(cam);
  paddles.draw(cam);
  ball.draw(cam);
}

void Scene_Game::activeDraw() {
  //nop
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

  auto* roid = asteroids.findCollision(ballCol);
  if(roid) {
    ball.setDirection({ ballCol.center.x - roid->collider.center.x, ballCol.center.y - roid->collider.center.y });
    asteroids.hit(*roid, { ballCol.center.x, ballCol.center.y });
  }

}

Scene* Scene_Game::processWinLoss() {
  if(reset) { return new Scene_Game(shared); }

  bool lose = false;
  float ballX = ball.getCollider().center.x;
  if(ballX < LEFT_OOB)  { lose = true; }
  if(ballX > RIGHT_OOB) { lose = true; }
  if(lose) {
    subScene = std::make_unique<Scene_Null>(shared);
    reset = true;
  }

  bool win = asteroids.population() == 0;
  if(win) {
    return new Scene_Game(shared);
  }

  return this;
}
