#pragma once
#include "simpleCollision.h"
#include "st_Asteroids.h"

struct Regions {
  SC::Rect middle;
  SC::Rect left;
  SC::Rect top;
  SC::Rect right;
  SC::Rect bottom;
};

struct ColliderSet {
  const Regions& regions;
  const SC::Rect& lPaddle;
  const SC::Rect& rPaddle;
  const Asteroids& asteroids;
};

