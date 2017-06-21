#pragma once
#include "vcl_Scene.h"
#include "st_SharedState.h"

class SubScene_BallOut : public Scene {
public:
  SubScene_BallOut(SharedState& shared) :
    Scene(shared),
    delay(3)
  {
    shared.gameState.lives--;
  }

  void activeDraw() override {}

  Scene* activeUpdate() override {
    delay -= shared.timer.getTickDT();
    if(delay < 0) { return nullptr; }
    return this;
  }

private:
  double delay;

};
