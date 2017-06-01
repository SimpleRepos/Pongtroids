#pragma once
#include "vcl_Scene.h"
#include "st_ShaderSet.h"
#include "cl_Texture.h"
#include "cl_Camera.h"
#include "cl_Transform.h"
#include "tcl_ConstantBuffer.h"
#include "cl_StaticMesh.h"
#include "st_Asteroid.h"
#include "st_Paddle.h"
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
  Texture tex;
  Camera cam;
  ConstantBuffer<DirectX::XMFLOAT4X4> cBuffer;
  StaticMesh roidMesh;
  Asteroid roid;
  Paddle rPaddle, lPaddle;
  StaticMesh squareMesh;
  Texture black;
  DirectX::XMFLOAT4X4 bgx;
  Ball ball;
  
};

