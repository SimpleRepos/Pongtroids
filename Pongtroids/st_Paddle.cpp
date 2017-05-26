#include "st_Paddle.h"
#include "st_Asteroid.h"

const SC::Rect Paddle::bounds = { -390, 290, 390, -290 };
const float Paddle::SPEED = 100;

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
  y = clamp(y, bounds.bottom, bounds.top);

  auto& x = xform.translation.x;
  if(shared.input.keyboard().buttons[VK_LEFT].held)   { x -= displacement; }
  if(shared.input.keyboard().buttons[VK_RIGHT].held)  { x += displacement; }
  x = clamp(x, bounds.left, bounds.right);

}
