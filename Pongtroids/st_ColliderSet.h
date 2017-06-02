#pragma once
#include "simpleCollision.h"
#include "st_Asteroids.h"
#include "st_Paddles.h"

struct Regions {
  SC::Rect middle;
  SC::Rect left;
  SC::Rect top;
  SC::Rect right;
  SC::Rect bottom;
};

struct ColliderSet {
  const Regions& regions;
  const Paddles& paddles;
  const Asteroids& asteroids;
};

