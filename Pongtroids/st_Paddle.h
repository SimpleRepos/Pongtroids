#pragma once
#include "cl_Transform.h"
#include "simpleCollision.h"
#include "st_SharedState.h"

struct Paddle {
  static constexpr float LEFT_X = -2.5f;
  static constexpr float RIGHT_X = 2.5f;

  static constexpr float    TOP_BOUND =  1.5f;
  static constexpr float BOTTOM_BOUND = -1.5f;

  static constexpr float SPEED = 3;

  void update(SharedState& shared, float dt);

  Transform xform;
  SC::Rect collider;

};

