#include "cl_Ball.h"
#include "cl_Graphics.h"
#include "cl_Camera.h"

using namespace DirectX;

Ball::Ball(SharedState& shared, RenderProgram<DirectX::XMFLOAT4X4>& spriteProg, DirectX::XMFLOAT2 startPos) :
  shared(shared),
  resetPos(startPos),
  collider{{ startPos.x, startPos.y }, RADIUS},
  tex(shared.factory.createTexture(L"../Assets/ball.png")),
  prog(&spriteProg),
  wallBounce(shared.audio.genSound("../Assets/350905__cabled-mess__jump-c-05.wav"))
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
  if(velocity.y > 0 && ((collider.center.y + collider.radius) > SCREEN_BOT)) {
    velocity.y = -velocity.y;
    wallBounce.play();
  }
  if(velocity.y < 0 && ((collider.center.y - collider.radius) < SCREEN_TOP)) {
    velocity.y = -velocity.y;
    wallBounce.play();
  }

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

  //hack to make more pleasing behavior for deflections at very steep angles
  if(signbit(vel.m128_f32[0]) == signbit(velocity.x)) {
    vel.m128_f32[0] = -vel.m128_f32[0];
  }

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

DebugBall::DebugBall(SharedState& shared, RenderProgram<DirectX::XMFLOAT4X4>& spriteProg, DirectX::XMFLOAT2 startPos) :
  Ball(shared, spriteProg, startPos),
  testing(-1), aiming(false),
  mpos(400, 300, 0)
{
  //nop
}

void DebugBall::update(float dt) {
  if(testing >= 0) {
    Ball::update(dt);
    testing -= dt;
    return;
  }

  auto mouse = shared.input.mouse();

  if(mouse.buttons[0].held) {
    aiming = true;
  }
  else {
    if(aiming) {
      aiming = false;

      DirectX::XMFLOAT2 aim(
        (mouse.axes[Input::Mouse::CURSOR_X] - RADIUS) - xform.translation.x,
        (mouse.axes[Input::Mouse::CURSOR_Y] - RADIUS) - xform.translation.y
      );
      setDirection(aim);

      testing = TEST_DURATION;
    }
    else {
      xform.translation.x = mouse.axes[Input::Mouse::CURSOR_X] - RADIUS;
      xform.translation.y = mouse.axes[Input::Mouse::CURSOR_Y] - RADIUS;
      collider.center.x = xform.translation.x + RADIUS;
      collider.center.y = xform.translation.y + RADIUS;
    }
  }

}
