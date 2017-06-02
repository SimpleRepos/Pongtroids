#pragma once
#include "vcl_Scene.h"
#include "st_ShaderSet.h"
#include "cl_Texture.h"
#include "cl_Camera.h"
#include "cl_Transform.h"
#include "tcl_ConstantBuffer.h"
#include "cl_StaticMesh.h"
#include "st_Asteroids.h"
#include "st_Paddles.h"
#include "st_Ball.h"
#include "st_ColliderSet.h"

class Scene_Game : public Scene {
public:
  Scene_Game(SharedState& shared);
  Scene* activeUpdate() override;
  void activeDraw() override;

  const Regions regions;
  ColliderSet colliderSet;

private:
  ShaderSet shaders;
  Camera cam;
  ConstantBuffer<DirectX::XMFLOAT4X4> cBuffer;

  Asteroids asteroids;
  Paddles paddles;
  Ball ball;

  StaticMesh squareMesh;
  Texture black;
  Texture white;
  DirectX::XMFLOAT4X4 bgx;
  
};

