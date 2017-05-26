#include "st_Paddle.h"
#include "st_Asteroid.h"
#include "Scene_Game.h"

namespace {
  const Scene_Game::Regions& regions = Scene_Game::regions;
}

const float Paddle::SPEED = 200;

namespace {
  float clamp(float val, float min, float max) {
    if(val < min) { val = min; }
    if(val > max) { val = max; }
    return val;
  }
}

void Paddle::update(SharedState& shared, float dt) {
  auto& y = xform.translation.y;

  if(shared.input.keyboard().buttons[VK_UP].held)   { y -= SPEED * dt; }
  if(shared.input.keyboard().buttons[VK_DOWN].held) { y += SPEED * dt; }

  y = clamp(y, regions.middle.top, regions.middle.bottom - collider.height());

  collider.y(y + xform.scale.y);
}
