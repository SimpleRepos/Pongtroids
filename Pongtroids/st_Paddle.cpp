#include "st_Paddle.h"
#include "Scene_Game.h"

const float Paddle::SPEED = 200;

namespace {
  float clamp(float val, float min, float max) {
    if(val < min) { val = min; }
    if(val > max) { val = max; }
    return val;
  }
}

Paddle::Paddle(float x, float y, float width, float height, const SC::Rect& bounds) :
  TOP_BOUND(bounds.top),
  BOT_BOUND(bounds.bottom)
{
  collider.x(x);
  collider.y(y);
  collider.width(width);
  collider.height(height);
  xform.translation.x = x;
  xform.translation.y = y;
  xform.scale.x = width;
  xform.scale.y = height;
}

Paddle& Paddle::operator=(const Paddle& other) {
  float* tbp = const_cast<float*>(&TOP_BOUND);
  *tbp = other.TOP_BOUND;
  float* bbp = const_cast<float*>(&BOT_BOUND);
  *bbp = other.BOT_BOUND;

  xform = other.xform;
  collider = other.collider;

  return *this;
}

void Paddle::update(float dt, SharedState& shared) {
  auto& y = xform.translation.y;

  if(shared.input.keyboard().buttons[VK_UP].held)   { y -= SPEED * dt; }
  if(shared.input.keyboard().buttons[VK_DOWN].held) { y += SPEED * dt; }

  y = clamp(y, TOP_BOUND, BOT_BOUND - collider.height());

  collider.y(y);
}

void Paddle::update(const Paddle& master) {
  float x = xform.translation.x;
  *this = master;
  xform.translation.x = x;
  collider.x(x);
}
