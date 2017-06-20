#include "st_Ball.h"
#include "cl_Graphics.h"
#include "cl_Camera.h"

using namespace DirectX;

Ball::Ball(SharedState& shared, RenderProgram<DirectX::XMFLOAT4X4>& spriteProg, DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 direction) :
  shared(shared),
  xform({ position.x, position.y, 0 }, { RADIUS * 2, RADIUS * 2, 1 }),
  velocity(direction.x * SPEED, direction.y * SPEED),
  collider{{ position.x, position.y }, RADIUS},
  tex(shared.factory.createTexture(L"../Assets/ball.png")),
  prog(&spriteProg)
{
  //nop
}

void Ball::update(float dt) {
  //integrate
  xform.translate(XMVectorScale(XMLoadFloat2(&velocity), dt));
  collider.center.x = xform.translation.x + RADIUS;
  collider.center.y = xform.translation.y + RADIUS;

  static const float SCREEN_TOP = 0;
  static const float SCREEN_BOT = (float)shared.gfx.VIEWPORT_DIMS.height;

  //bounce off top and bottom
  if(velocity.y > 0 && ((collider.center.y + collider.radius) > SCREEN_BOT)) { velocity.y = -velocity.y; }
  if(velocity.y < 0 && ((collider.center.y - collider.radius) < SCREEN_TOP)) { velocity.y = -velocity.y; }

}

void Ball::draw(Camera& cam) {
  prog->set();
  prog->cBuffer.object = cam.getTransposedWVP(xform);
  prog->cBuffer.update();
  tex.set(0);
  shared.gfx.draw(4);
}

void Ball::setDirection(const DirectX::XMFLOAT2& dir) {
  XMStoreFloat2(&velocity, XMVectorScale(XMVector2Normalize(XMLoadFloat2(&dir)), SPEED));
}
