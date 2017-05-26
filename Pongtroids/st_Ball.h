#pragma once
#include "cl_Transform.h"
#include "simpleCollision.h"

struct Ball {
  void update(float dt) {
    /*
    ~~_
    integrate
    check bounds
    check paddle in direction of travel
    check roids
    */
  }

  Transform xform;
  SC::Circle collider;

};
