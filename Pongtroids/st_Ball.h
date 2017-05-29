#pragma once
#include "cl_Transform.h"
#include "simpleCollision.h"
#include "st_Paddle.h"

struct Ball {
  void update(float dt, Paddle& rPaddle);

  Transform xform;
  DirectX::XMFLOAT2 velocity;
  SC::Circle collider;

};

