#include "st_Asteroid.h"
#include "Scene_Game.h"
#include "st_ColliderSet.h"

using namespace DirectX;

Asteroid::Asteroid(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 direction, float size, const Regions& regions) :
  regions(regions),
  size(size),
  velocity{ direction.x * SPEED, direction.y * SPEED },
  alive(true)
{
  collider.radius = size;

  xform.mulScale(size);
  xform.translation.x = position.x;
  xform.translation.y = position.y;
}

void Asteroid::update(float dt) {
  XMVECTOR vel = XMLoadFloat2(&velocity);
  xform.translate(XMVectorScale(vel, dt));
  xform.rotate(XMVector2Orthogonal(vel), dt);

  collider.center = SC::Point{ xform.translation.x, xform.translation.y };

  if(velocity.x < 0 && SC::testOverlap(collider, regions.left))  {
    velocity.x = -velocity.x;
  }
  if(velocity.x > 0 && SC::testOverlap(collider, regions.right)) {
    velocity.x = -velocity.x;
  }

  if(!SC::testOverlap(collider, regions.middle)) {
    float displace = regions.middle.height() + size;
    if(velocity.y > 0) { displace = -displace; }
    xform.translation.y += displace;
  }
}
