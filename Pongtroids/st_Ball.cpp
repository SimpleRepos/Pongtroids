#include "st_Ball.h"
#include "ns_GameScene.h"
#include "cl_Graphics.h"

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
  if(xform.translation.x < regions.left.left)   { MessageBoxA(0, "You lost.", 0, 0); PostQuitMessage(0); }
  if(xform.translation.x > regions.right.right) { MessageBoxA(0, "You lost.", 0, 0); PostQuitMessage(0); }

  //check paddle in direction of travel
  if(velocity.x > 0 && SC::testOverlap(entities.paddles.getCollider(Paddles::RIGHT), collider)) {
    velocity.x = -velocity.x;
  }
  if(velocity.x < 0 && SC::testOverlap(entities.paddles.getCollider(Paddles::LEFT), collider)) {
    velocity.x = -velocity.x;
  }

  //check roids
  for(auto& roid : entities.asteroids.asteroids) {
    if(SC::testOverlap(collider, roid.collider)) {
      XMVECTOR myPos = XMLoadFloat2(reinterpret_cast<const DirectX::XMFLOAT2*>(&collider.center));
      XMVECTOR roidPos = XMLoadFloat2(reinterpret_cast<const DirectX::XMFLOAT2*>(&roid.collider.center));
      XMVECTOR dir = XMVector2Normalize(DirectX::XMVectorSubtract(myPos, roidPos));
      XMStoreFloat2(&velocity, XMVectorScale(dir, SPEED));

      //~~_ roid.hit();
    }
  }
}

void Ball::draw(GameScene::RenderProgram& prog) {
  tex.set(0);

  prog.cBuffer.object = prog.cam.getTransposedWVP(xform);
  prog.cBuffer.update();
  shared.gfx.draw(prog.squareMesh);
}

