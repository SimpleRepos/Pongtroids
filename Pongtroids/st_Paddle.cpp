#include "st_Paddle.h"

void Paddle::update(SharedState& shared, float dt) {
  float motion = 0;
  if(shared.input.keyboard().buttons[VK_UP].held)   { motion += SPEED; }
  if(shared.input.keyboard().buttons[VK_DOWN].held) { motion -= SPEED; }
  motion *= dt;

  auto& y = xform.translation.y;
  y += motion;
  if(y > TOP_BOUND)    { y = TOP_BOUND; }
  if(y < BOTTOM_BOUND) { y = BOTTOM_BOUND; }

}
