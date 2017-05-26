#include "st_Paddle.h"
#include "st_Asteroid.h"

const SC::Rect Paddle::bounds = { 0, 300, 0, -300 };
const float Paddle::SPEED = 200;

namespace {
  float clamp(float val, float min, float max) {
    if(val < min) { val = min; }
    if(val > max) { val = max; }
    return val;
  }
}

void Paddle::update(SharedState& shared, float dt) {
  const float displacement = SPEED * dt;

  auto& y = xform.translation.y;
  if(shared.input.keyboard().buttons[VK_UP].held)   { y += displacement; }
  if(shared.input.keyboard().buttons[VK_DOWN].held) { y -= displacement; }
  y = clamp(y, bounds.bottom + xform.scale.y, bounds.top - xform.scale.y);

  collider.y(y + xform.scale.y);
}
