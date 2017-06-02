#include "st_Ball.h"
#include "Scene_Game.h"
#include "st_ColliderSet.h"

using namespace DirectX;

Ball::Ball(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 direction) {
  XMStoreFloat2(&velocity, XMVectorScale(XMLoadFloat2(&direction), SPEED));
  xform.translation = { position.x, position.y, 0 };
  collider.radius = RADIUS;
  xform.mulScale(RADIUS * 2);
}

void Ball::update(float dt, const ColliderSet& colliders) {
  //integrate
  xform.translate(XMVectorScale(XMLoadFloat2(&velocity), dt));
  collider.center.x = xform.translation.x + RADIUS;
  collider.center.y = xform.translation.y + RADIUS;

  //check bounds
  if(velocity.y > 0 && SC::testOverlap(colliders.regions.bottom, collider)) { velocity.y = -velocity.y; }
  if(velocity.y < 0 && SC::testOverlap(colliders.regions.top,    collider)) { velocity.y = -velocity.y; }
  if(xform.translation.x < colliders.regions.left.left)   {  } //~~_
  if(xform.translation.x > colliders.regions.right.right) {  } //~~_

  //check paddle in direction of travel
  if(velocity.x > 0 && SC::testOverlap(colliders.paddles.getCollider(Paddles::RIGHT), collider)) {
    velocity.x = -velocity.x;
  }
  if(velocity.x < 0 && SC::testOverlap(colliders.paddles.getCollider(Paddles::LEFT), collider)) {
    velocity.x = -velocity.x;
  }

  //check roids
  for(auto& roid : colliders.asteroids.asteroids) {
    if(SC::testOverlap(collider, roid.collider)) {
      XMVECTOR myPos = XMLoadFloat2(reinterpret_cast<const DirectX::XMFLOAT2*>(&collider.center));
      XMVECTOR roidPos = XMLoadFloat2(reinterpret_cast<const DirectX::XMFLOAT2*>(&roid.collider.center));
      XMVECTOR dir = XMVector2Normalize(DirectX::XMVectorSubtract(myPos, roidPos));
      XMStoreFloat2(&velocity, XMVectorScale(dir, SPEED));

      //~~_ roid.hit();
    }
  }
}
