#include "st_Ball.h"
#include "ns_GameScene.h"
#include "cl_Graphics.h"
#include "cl_Camera.h"

using namespace DirectX;

Ball::Ball(SharedState& shared, DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 direction) :
  shared(shared),
  xform({ position.x, position.y, 0 }, { RADIUS * 2, RADIUS * 2, 1 }),
  velocity(direction.x * SPEED, direction.y * SPEED),
  collider{{ position.x, position.y }, RADIUS},
  tex(shared.factory.createTexture(L"../Assets/ball.png"))
{
  //nop
}

void Ball::update(float dt, GameScene::Entities& entities, const GameScene::Regions& regions) {
  //integrate
  xform.translate(XMVectorScale(XMLoadFloat2(&velocity), dt));
  collider.center.x = xform.translation.x + RADIUS;
  collider.center.y = xform.translation.y + RADIUS;

  //check bounds
  if(velocity.y > 0 && SC::testOverlap(regions.bottom, collider)) { velocity.y = -velocity.y; }
  if(velocity.y < 0 && SC::testOverlap(regions.top,    collider)) { velocity.y = -velocity.y; }

  //check paddles
  Paddles::Side side = (velocity.x > 0) ? Paddles::RIGHT : Paddles::LEFT;
  if(SC::testOverlap(entities.paddles.getCollider(side), collider)) {
    float theta = entities.paddles.getDeflectionAngle(side, xform.y);
    int sign = (2 * side) - 1; //LEFT is negative and RIGHT is positive
    velocity = { -sign * cosf(theta) * SPEED, sinf(theta) * SPEED }; //vel.x should be reversed
  }

  //check roids
  for(auto& roid : entities.asteroids.asteroids) {
    if(SC::testOverlap(collider, roid.collider)) {
      XMVECTOR myPos = XMLoadFloat2(reinterpret_cast<const DirectX::XMFLOAT2*>(&collider.center));
      XMVECTOR roidPos = XMLoadFloat2(reinterpret_cast<const DirectX::XMFLOAT2*>(&roid.collider.center));
      XMVECTOR dir = XMVector2Normalize(DirectX::XMVectorSubtract(myPos, roidPos));
      XMStoreFloat2(&velocity, XMVectorScale(dir, SPEED));

      roid.hit({ xform.x, xform.y }, entities.asteroids);
    }
  }
}

void Ball::draw(RenderProgram<DirectX::XMFLOAT4X4>& prog, Camera& cam) {
  prog.set();
  prog.cBuffer.object = cam.getTransposedWVP(xform);
  prog.cBuffer.update();
  tex.set(0);
  shared.gfx.draw(4);
}
