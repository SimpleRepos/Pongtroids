#include "st_Ball.h"
#include "Scene_Game.h"

void Ball::update(float dt, Paddle& rPaddle) {
  const Scene_Game::Regions& regions = Scene_Game::regions;

  //integrate
  xform.translate(DirectX::XMVectorScale(DirectX::XMLoadFloat2(&velocity), dt));
  collider.center.x = xform.translation.x;
  collider.center.y = xform.translation.y;

  //check bounds
  if(velocity.y > 0 && SC::testOverlap(regions.bottom, collider)) {
    velocity.y = -velocity.y;
  }
  if(velocity.y < 0 && SC::testOverlap(regions.top,    collider)) {
    velocity.y = -velocity.y;
  }

  //check paddle in direction of travel
  if(velocity.x > 0 && SC::testOverlap(rPaddle.collider, collider)) {
    velocity.x = -velocity.x;
  }
  if(velocity.x < 0 && xform.translation.x < 0 /*SC::testOverlap(lPaddle, collider)*/) {
    velocity.x = -velocity.x;
  }

  //check roids
  //~~_
}