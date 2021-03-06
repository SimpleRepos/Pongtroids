#include "Scene_Game.h"
#include "st_SharedState.h"
#include "cl_GfxFactory.h"
#include <vector>
#include <cmath>
#include <fstream>
#include "ns_Vertex.h"
#include "cl_Window.h"
#include "SubScene_BallOut.h"

Scene_Game::Scene_Game(SharedState& shared) : 
  Scene(shared),
  spriteProg{
    ShaderSet(
      shared.factory,
      "../Assets/vs_sprite.cso", D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
      "", "", "",
      RasterizerState::DEFAULT_DESC,
      "../Assets/ps_texture.cso"
    ),
    shared.factory.createConstantBuffer<DirectX::XMFLOAT4X4>()
  },
  cam(),
  bg(shared, spriteProg),
  asteroids(shared, 3),
  paddles(shared, spriteProg),
  ball(shared, spriteProg, { 400, 300 }),
  scoreBoard(shared),
  LEFT_OOB(0),
  RIGHT_OOB((float)shared.gfx.VIEWPORT_DIMS.width),
  cheatyTimeScale(1),
  paddleBounce(shared.audio.genSound("../Assets/361230__someguy22__8-bit-bounce.wav"))
{
  paddleBounce.volume(0.25f);

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

  shared.audio.bgmVolume(0.6f);
  shared.audio.setBGMTrack("../Assets/Cupid's Revenge.mp3");

  shared.win.addKeyFunc(VK_ADD,      [this](HWND, LPARAM) { cheatyTimeScale *= 1.25f; });
  shared.win.addKeyFunc(VK_SUBTRACT, [this](HWND, LPARAM) { cheatyTimeScale *= 0.8f; });
  shared.win.addKeyFunc(VK_ESCAPE,   [this](HWND, LPARAM) { PostQuitMessage(0); });
}

void Scene_Game::passiveUpdate() {
  float dt = (float)shared.timer.getTickDT() * cheatyTimeScale;
  bg.update(dt);
  asteroids.update(dt);
  paddles.update(dt);
  scoreBoard.update();
}

Scene* Scene_Game::activeUpdate() {
  float dt = (float)shared.timer.getTickDT() * cheatyTimeScale;

  ball.update(dt);

  ballVPaddles();
  ballVRoids();
  ballVBounds();

  if(shared.gameState.lives < 0)   { return nullptr; }
  if(asteroids.population() == 0)  { return nullptr; }

  return this;
}

void Scene_Game::passiveDraw() {
  shared.gfx.clear(ColorF::CYAN);
  bg.draw(cam);
  asteroids.draw(cam);
  paddles.draw(cam);
  scoreBoard.draw();  
}

void Scene_Game::activeDraw() {
  ball.draw(cam);
}

void Scene_Game::ballVPaddles() {
  Paddles::Side side = (ball.getVelocity().x > 0) ? Paddles::RIGHT : Paddles::LEFT;

  auto& ballCol = ball.getCollider();

  if(SC::testOverlap(ballCol, paddles.getCollider(side))) {
    ball.deflect(paddles.getDeflectionNormal(side, ballCol.center.y));
    paddleBounce.play();
    shared.gameState.resetMultiplier();
  }

}

void Scene_Game::ballVRoids() {
  auto& ballCol = ball.getCollider();

  auto* roid = asteroids.findCollision(ballCol);
  if(roid) {
    ball.deflect({ roid->collider.center.x - ballCol.center.x, roid->collider.center.y - ballCol.center.y });
    asteroids.hit(*roid, { ballCol.center.x, ballCol.center.y });

    scoreBoard.pushFlair(ballCol.center.x, ballCol.center.y);

    shared.gameState.scoreUp(POINTS_PER_HIT);
  }

}

void Scene_Game::ballVBounds() {
  bool out = false;
  float ballX = ball.getCollider().center.x;
  if(ballX < LEFT_OOB)  { out = true; }
  if(ballX > RIGHT_OOB) { out = true; }
  if(out) {
    subScene = std::make_unique<SubScene_BallOut>(shared);
    ball.reset();
    shared.gameState.resetMultiplier();
  }
}

