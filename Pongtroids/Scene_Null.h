#pragma once
#include "vcl_Scene.h"
#include "st_SharedState.h"

class Scene_Null : public Scene {
public:
  Scene_Null(SharedState& shared) : Scene(shared), delay(3) {}
  void activeDraw() override {}

  Scene* activeUpdate() override {
    delay -= shared.timer.getTickDT();
    if(delay < 0) { return nullptr; }
    return this;
  }

private:
  double delay;

};
