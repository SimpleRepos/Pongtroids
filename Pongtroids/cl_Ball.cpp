#include "cl_Ball.h"
#include "cl_Graphics.h"
#include "cl_Camera.h"

using namespace DirectX;

Ball::Ball(SharedState& shared, RenderProgram<DirectX::XMFLOAT4X4>& spriteProg, DirectX::XMFLOAT2 startPos) :
  shared(shared),
  resetPos(startPos),
  collider{{ startPos.x, startPos.y }, RADIUS},
  tex(shared.factory.createTexture(L"../Assets/ball.png")),
  prog(&spriteProg)
{
  reset();
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

void Ball::deflect(const DirectX::XMFLOAT2& normal) {
  XMVECTOR nrm = XMVector2Normalize(XMLoadFloat2(&normal));
  XMVECTOR vel = XMLoadFloat2(&velocity);

  float dot = XMVector2Dot(nrm, vel).m128_f32[0];
  XMVECTOR deflector(XMVectorScale(nrm, 2 * dot));

  vel = XMVectorSubtract(vel, deflector);

  XMStoreFloat2(&velocity, vel);
}

void Ball::setDirection(const DirectX::XMFLOAT2& dir) {
  XMStoreFloat2(&velocity, XMVectorScale(XMVector2Normalize(XMLoadFloat2(&dir)), SPEED));
}

void Ball::reset() {
  xform = Transform({ resetPos.x, resetPos.y, 0 }, { RADIUS * 2, RADIUS * 2, 1 });
  velocity = Utility::randDirVec(shared.rng);
  velocity.x *= SPEED;
  velocity.y *= SPEED;
}

