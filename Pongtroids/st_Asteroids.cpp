#include "st_Asteroids.h"
#include "ns_GameScene.h"

using namespace DirectX;
using namespace Utility;

Asteroids::Asteroids(SharedState& shared, const GameScene::Regions& regions, size_t count) :
  shared(shared),
  mesh(shared.factory.createStaticMeshFromOldMeshFileFormat("../Assets/asteroid.mesh")),
  tex(shared.factory.createTexture(L"../Assets/asteroid_diffuse.png"))
{
  RandomPositionGenerator posGen(regions.middle.left, regions.middle.top, regions.middle.right, regions.middle.bottom);

  for(size_t i = 0; i < count; i++) {
    float size = 50; //~~@ make this random in some range
    asteroids.emplace_back(size, posGen.randPos(shared.rng), randDirVec(shared.rng));
  }

}

void Asteroids::update(float dt, const GameScene::Regions& regions) {
  for(auto& roid : asteroids) {
    //move
    XMVECTOR vel = XMLoadFloat2(&roid.velocity);
    roid.xform.translate(XMVectorScale(vel, dt));
    roid.xform.rotate(XMVector2Orthogonal(vel), dt);
    roid.collider.center = SC::Point{ roid.xform.translation.x, roid.xform.translation.y };

    //bounce off left/right walls
    if(roid.velocity.x < 0 && SC::testOverlap(roid.collider, regions.left))  { roid.velocity.x = -roid.velocity.x; }
    if(roid.velocity.x > 0 && SC::testOverlap(roid.collider, regions.right)) { roid.velocity.x = -roid.velocity.x; }

    //loop around top/bottom walls
    if(!SC::testOverlap(roid.collider, regions.middle)) {
      float displace = regions.middle.height() + roid.SIZE;
      if(roid.velocity.y > 0) { displace = -displace; }
      roid.xform.translation.y += displace;
    }
  }
}

void Asteroids::draw(GameScene::RenderProgram& prog) {
  tex.set(0);
  for(auto& roid : asteroids) {
    prog.cBuffer.object = prog.cam.getTransposedWVP(roid.xform);
    prog.cBuffer.update();
    shared.gfx.draw(mesh);
  }
}

Asteroids::Asteroid::Asteroid(float size, DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 direction) : 
  SIZE(size),
  velocity(XMFLOAT2(direction.x * SPEED, direction.y * SPEED)),
  alive(true)
{
  xform.translate({ position.x, position.y, 0 });
  xform.mulScale(SIZE);
  collider.center = SC::Point{ position.x, position.y };
  collider.radius = SIZE;
}
