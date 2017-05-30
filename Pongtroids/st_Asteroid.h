#pragma once
#include "cl_Transform.h"
#include <DirectXMath.h>
#include "simpleCollision.h"

struct Regions;

struct Asteroid {
  Asteroid(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 direction, float size, const Regions& regions);
  void update(float dt);

  static constexpr float SPEED = 50;
  const Regions& regions;

  const float size;
  Transform xform;
  DirectX::XMFLOAT2 velocity;
  bool alive;
  SC::Circle collider;

};

