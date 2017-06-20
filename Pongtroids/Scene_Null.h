#pragma once
#include "vcl_Scene.h"
#include "st_SharedState.h"

class Scene_Null : public Scene {
public:
  Scene_Null(SharedState& shared) : Scene(shared) {}
  void activeDraw() override {}

  Scene* activeUpdate() override {
    if(shared.input.keyboard().buttons[VK_ESCAPE].triggered) { return nullptr; }
    return this;
  }

};
