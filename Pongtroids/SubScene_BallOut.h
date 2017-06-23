#pragma once
#include "vcl_Scene.h"
#include "st_SharedState.h"

class SubScene_BallOut : public Scene {
public:
  SubScene_BallOut(SharedState& shared);
  void activeDraw() override;
  Scene* activeUpdate() override;

private:
  double delay = 3;

};

SubScene_BallOut::SubScene_BallOut(SharedState& shared) : Scene(shared) {
  shared.gameState.lives--;
}

void SubScene_BallOut::activeDraw() {}

Scene* SubScene_BallOut::activeUpdate() {
  delay -= shared.timer.getTickDT();
  if(delay < 0) { return nullptr; }
  return this;
}

