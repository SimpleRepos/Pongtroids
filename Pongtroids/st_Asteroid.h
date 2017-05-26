#pragma once
#include "cl_Transform.h"
#include <DirectXMath.h>
#include "simpleCollision.h"

struct Asteroid {
  Asteroid(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 velocity, float size);
  void update(float dt);


  const float size;
  Transform xform;
  DirectX::XMFLOAT2 velocity;
  bool alive;
  SC::Circle collider;

};

