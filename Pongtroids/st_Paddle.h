#pragma once
#include "cl_Transform.h"
#include "simpleCollision.h"
#include "st_SharedState.h"

struct Paddle {
  Paddle(float x, float y, float width, float height, const SC::Rect& bounds);
  Paddle& operator=(const Paddle& other);

  void update(float dt, SharedState& shared);
  void update(const Paddle& master);

  Transform xform;
  SC::Rect collider;

  static const float SPEED;

  const float TOP_BOUND;
  const float BOT_BOUND;
};

