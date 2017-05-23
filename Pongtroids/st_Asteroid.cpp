#include "st_Asteroid.h"

using namespace DirectX;

SC::Rect Asteroid::bounds = { -2, 2.5f, 2, -2.5f };

void Asteroid::update(float dt) {
  XMVECTOR vel = XMLoadFloat2(&velocity);
  xform.translate(XMVectorScale(vel, dt));
  xform.rotate(XMVector2Orthogonal(vel), dt);

  //bounds interactions
  float transgress;
  //bounce off horizontally
  transgress = xform.translation.x - bounds.right;
  if(transgress > 0) {
    xform.translation.x = bounds.right - transgress;
    velocity.x = -velocity.x;
  }
  transgress = bounds.left - xform.translation.x;
  if(transgress > 0) {
    xform.translation.x = bounds.left + transgress;
    velocity.x = -velocity.x;
  }
  //loop around vertically
  transgress = xform.translation.y - bounds.top;
  if(transgress > 0) { xform.translation.y = bounds.bottom + transgress; }
  transgress = bounds.bottom - xform.translation.y;
  if(transgress > 0) { xform.translation.y = bounds.top - transgress; }

}

Asteroid::Asteroid(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 velocity, Size size) :
  size(size),
  velocity(velocity),
  alive(true)
{
  switch(size) {
  case LARGE:  collider.radius = 0.5f;   break;
  case MEDIUM: collider.radius = 0.25f;  break;
  case SMALL:  collider.radius = 0.125f; break;
  };

  xform.mulScale(collider.radius);
  xform.translation.x = position.x;
  xform.translation.y = position.y;
  
}
