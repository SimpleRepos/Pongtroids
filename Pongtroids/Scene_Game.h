#pragma once
#include "vcl_Scene.h"
#include "cl_StaticVertexBuffer.h"
#include "cl_Texture.h"
#include "cl_Camera.h"
#include "cl_Transform.h"

class Scene_Game : public Scene {
public:
  Scene_Game(SharedState& shared);
  Scene* update() override;
  void draw() override;

private:
  StaticVertexBuffer verts;
  Texture tex;
  Transform xform;
  Camera cam;

};

