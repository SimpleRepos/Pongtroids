#include "st_Asteroid.h"

using namespace DirectX;

SC::Rect Asteroid::bounds = { -350, 300, 350, -300 };

void Asteroid::update(float dt) {
  XMVECTOR vel = XMLoadFloat2(&velocity);
  xform.translate(XMVectorScale(vel, dt));
  xform.rotate(XMVector2Orthogonal(vel), dt);

  //bounds interactions
  float transgress;
  //bounce off horizontally
  transgress = (xform.translation.x + collider.radius) - bounds.right;
  if(transgress > 0) {
    xform.translation.x -= 2 * transgress;
    velocity.x = -velocity.x;
  }

  transgress = bounds.left - (xform.translation.x - collider.radius);
  if(transgress > 0) {
    xform.translation.x += 2 * transgress;
    velocity.x = -velocity.x;
  }

  //loop around vertically
  transgress = xform.translation.y - (bounds.top + collider.radius);
  if(transgress > 0) { xform.translation.y = (bounds.bottom - collider.radius) + transgress; }
  transgress = (bounds.bottom - collider.radius) - xform.translation.y;
  if(transgress > 0) { xform.translation.y = (bounds.top + collider.radius) - transgress; }

}

Asteroid::Asteroid(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 velocity, Size size) :
  size(size),
  velocity(velocity),
  alive(true)
{
  switch(size) {
  case LARGE:  collider.radius = 50; break;
  case MEDIUM: collider.radius = 25;  break;
  case SMALL:  collider.radius = 12;  break;
  };

  xform.mulScale(collider.radius);
  xform.translation.x = position.x;
  xform.translation.y = position.y;
  
}
