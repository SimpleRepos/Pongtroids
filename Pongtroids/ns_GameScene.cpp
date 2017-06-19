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
  asteroids(shared, regions.middle, numRoids),
  paddles(shared, spriteProg),
  ball(shared, spriteProg, { 400, 300 }, Utility::randDirVec(shared.rng))
{
  //nop
}

void GameScene::Entities::update(float dt, const GameScene::Regions& regions) {
  asteroids.update(dt);
  paddles.update(dt);
  ball.update(dt, *this, regions);
}

void GameScene::Entities::draw(Camera& cam) {
  asteroids.draw(cam);
  paddles.draw(cam);
  ball.draw(cam);
}

