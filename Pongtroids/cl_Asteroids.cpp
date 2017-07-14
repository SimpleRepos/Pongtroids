#include "cl_Asteroids.h"
#include <algorithm>
#include <functional>
#include "cl_Camera.h"

using namespace DirectX;
using namespace Utility;

Asteroids::Asteroids(SharedState& shared, size_t count) :
  centerRegion{ HBOUND_MARGIN, 0, (float)shared.gfx.VIEWPORT_DIMS.width - HBOUND_MARGIN, (float)shared.gfx.VIEWPORT_DIMS.height },
  shared(shared),
  hitSound(shared.audio.genSound("../Assets/83976__theredshore__punch.wav")),
  mesh(shared.factory.createStaticMeshFromOldMeshFileFormat("../Assets/asteroid.mesh")),
  tex(shared.factory.createTexture(L"../Assets/asteroid_diffuse.png")),
  prog{
    ShaderSet(
      shared.factory,
      "../Assets/vs_simpleXform.cso", D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
      "", "", "",
      RasterizerState::DEFAULT_DESC, "../Assets/ps_texture.cso"
    ),
    shared.factory.createConstantBuffer<DirectX::XMFLOAT4X4>()
  }
{
  RandomPositionGenerator posGen(centerRegion.left, centerRegion.top, centerRegion.right, centerRegion.bottom);
  std::uniform_real_distribution<float> sizeDist(Asteroid::MIN_START_SIZE, Asteroid::MAX_START_SIZE);

  for(size_t i = 0; i < count; i++) {
    float size = sizeDist(shared.rng);
    auto pos = posGen.randPos(shared.rng);
    enqueue(size, { pos.x, pos.y, 0 }, randDirVec(shared.rng));
  }
}

void Asteroids::update(float dt) {
  for(auto& roid : asteroids) {
    //move
    XMVECTOR vel = XMLoadFloat2(&roid.velocity);
    roid.xform.translate(XMVectorScale(vel, dt));
    roid.xform.rotate(XMVector2Orthogonal(vel), dt);
    roid.collider.center = SC::Point{ roid.xform.translation.x, roid.xform.translation.y };

    //bounce off left/right walls
    if(roid.velocity.x < 0 && (roid.collider.center.x - roid.collider.radius) < centerRegion.left)  { roid.velocity.x = -roid.velocity.x; }
    if(roid.velocity.x > 0 && (roid.collider.center.x + roid.collider.radius) > centerRegion.right) { roid.velocity.x = -roid.velocity.x; }

    //loop around top/bottom walls
    if(!SC::testOverlap(roid.collider, centerRegion)) {
      float displace = centerRegion.height() + roid.collider.radius;
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

size_t Asteroids::population() const {
  return asteroids.size() + queue.size();
}

void Asteroids::hit(Asteroid& roid, DirectX::XMFLOAT2 ballPos) {
  roid.alive = false;

  hitSound.play();

  float newSize = roid.collider.radius / 2;
  if(newSize < Asteroid::MIN_SIZE) { return; }

  XMVECTOR vDir = XMVector2Orthogonal(
    XMVector2Normalize(
      XMLoadFloat2(&ballPos) - XMLoadFloat3(&roid.xform.translation)
    )
  );

  XMFLOAT2 posDir, negDir;
  XMStoreFloat2(&posDir,  vDir);
  XMStoreFloat2(&negDir, -vDir);

  enqueue(newSize, roid.xform.translation, posDir);
  enqueue(newSize, roid.xform.translation, negDir);
}

Asteroids::Asteroid* Asteroids::findCollision(const SC::Circle& ballCol) {
  for(auto& roid : asteroids) {
    if(SC::testOverlap(ballCol, roid.collider)) { return &roid; }
  }
  return nullptr;
}

void Asteroids::enqueue(float size, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT2& dir) {
  queue.emplace_back(Asteroid{ Transform(), { dir.x * Asteroid::SPEED, dir.y * Asteroid::SPEED }, true, { { pos.x, pos.y }, size } });
  auto& roid = queue.back();
  roid.xform.translation = pos;
  roid.xform.mulScale(size);
}

void Asteroids::clean() {
  auto iter = std::remove_if(asteroids.begin(), asteroids.end(), [](const Asteroids::Asteroid& roid) -> bool { return !roid.alive; });
  if(iter != asteroids.end()) { asteroids.erase(iter); }}

void Asteroids::spawnQueued() {
  asteroids.insert(asteroids.end(), queue.begin(), queue.end());
  queue.clear();
}

