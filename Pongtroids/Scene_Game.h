#pragma once
#include "vcl_Scene.h"

class Scene_Game : public Scene {
public:
  Scene_Game(SharedState& shared);
  Scene* update() override;
  void draw() override;

private:

};

