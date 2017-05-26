#pragma once
#include "cl_Transform.h"
#include "simpleCollision.h"
#include "st_SharedState.h"

struct Paddle {
  void update(SharedState& shared, float dt);

  Transform xform;
  SC::Rect collider;

  static const SC::Rect bounds;
  static const float SPEED;

};

