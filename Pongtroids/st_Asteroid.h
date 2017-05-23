#pragma once
#include "cl_Transform.h"
#include <DirectXMath.h>
#include "simpleCollision.h"

struct Asteroid {
  enum Size { SMALL, MEDIUM, LARGE };

  Asteroid(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 velocity, Size size = LARGE);
  void update(float dt);


  Size size;
  Transform xform;
  DirectX::XMFLOAT2 velocity;
  bool alive;
  SC::Circle collider;

  static SC::Rect bounds;

};

