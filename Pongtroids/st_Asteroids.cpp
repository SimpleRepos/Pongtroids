#include "st_Asteroids.h"
#include "ns_GameScene.h"
#include <algorithm>
#include <functional>
#include "cl_Camera.h"

using namespace DirectX;
using namespace Utility;

Asteroids::Asteroids(SharedState& shared, const GameScene::Regions& regions, size_t count) :
  shared(shared),
  mesh(shared.factory.createStaticMeshFromOldMeshFileFormat("../Assets/asteroid.mesh")),
  tex(shared.factory.createTexture(L"../Assets/asteroid_diffuse.png")),
  prog{
    ShaderSet(
      shared.factory,
      "../Assets/vs_temporarySimpleTransform.cso", D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
      "", "", "",
      RasterizerState::DEFAULT_DESC, "../Assets/ps_texture.cso"
    ),
    shared.factory.createConstantBuffer<DirectX::XMFLOAT4X4>()
  }
{
  RandomPositionGenerator posGen(regions.middle.left, regions.middle.top, regions.middle.right, regions.middle.bottom);
  std::uniform_real_distribution<float> sizeDist(50, 75);

  for(size_t i = 0; i < count; i++) {
    float size = sizeDist(shared.rng);
    auto pos = posGen.randPos(shared.rng);
    asteroids.emplace_back(RoidArgs{ size, { pos.x, pos.y, 0 }, randDirVec(shared.rng) });
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
      float displace = regions.middle.height() + roid.collider.radius;
      if(roid.velocity.y > 0) { displace = -displace; }
      roid.xform.translation.y += displace;
    }
  }

  clean();
  spawnQueued();
}

void Asteroids::draw(Camera& cam) {
  prog.set();
  tex.set(0);
  for(auto& roid : asteroids) {
    prog.cBuffer.object = cam.getTransposedWVP(roid.xform);
    prog.cBuffer.update();
    shared.gfx.draw(mesh);
  }
}

void Asteroids::enqueue(float size, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT2& dir) {
  queue.push_back(RoidArgs{size, pos, dir});
}

size_t Asteroids::population() const {
  return asteroids.size() + queue.size();
}

Asteroids::Asteroid::Asteroid(const RoidArgs& args) : 
  velocity(XMFLOAT2(args.dir.x * SPEED, args.dir.y * SPEED)),
  alive(true)
{
  xform.translation = args.pos;
  xform.mulScale(args.size);
  collider.center = SC::Point{ args.pos.x, args.pos.y };
  collider.radius = args.size;
}

void Asteroids::Asteroid::hit(DirectX::XMFLOAT2 ballPos, Asteroids& asteroids) {
  alive = false;

  float newSize = collider.radius / 2;
  if(newSize < MIN_SIZE) { return; }

  XMVECTOR vDir = XMVector2Orthogonal(
    XMVector2Normalize(
      XMLoadFloat2(&ballPos) - XMLoadFloat3(&xform.translation)
    )
  );

  XMFLOAT2 posDir, negDir;
  XMStoreFloat2(&posDir,  vDir);
  XMStoreFloat2(&negDir, -vDir);

  asteroids.enqueue(newSize, xform.translation, posDir);
  asteroids.enqueue(newSize, xform.translation, negDir);
}

namespace {
  bool isDead(const Asteroids::Asteroid& roid) {
    return !roid.alive;
  }
}

void Asteroids::clean() {
  auto iter = std::remove_if(asteroids.begin(), asteroids.end(), isDead);
  if(iter != asteroids.end()) { asteroids.erase(iter); }}

void Asteroids::spawnQueued() {
  for(auto& args : queue) { asteroids.emplace_back(args); }
  queue.clear();
}

