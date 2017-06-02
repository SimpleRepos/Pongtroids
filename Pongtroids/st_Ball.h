#pragma once
#include "cl_Transform.h"
#include "simpleCollision.h"
#include "st_Paddles.h"

struct ColliderSet;

struct Ball {
  Ball(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 velocity);
  void update(float dt, const ColliderSet& colliders);

  static constexpr float RADIUS = 5;
  static constexpr float SPEED = 200;

  Transform xform;
  DirectX::XMFLOAT2 velocity;
  SC::Circle collider;

};

