#pragma once
#include "vcl_Scene.h"
#include "st_SharedState.h"
#include "ns_GameScene.h"
#include "tst_RenderProgram.h"
#include "cl_Camera.h"
#include "cl_Texture.h"
#include <DirectXMath.h>

class Scene_Game : public Scene {
public:
  Scene_Game(SharedState& shared);
  Scene* activeUpdate() override;
  void activeDraw() override;

private:
  const GameScene::Regions regions;
  RenderProgram<DirectX::XMFLOAT4X4> renderProg;
  Camera cam;
  StaticMesh squareMesh;
  GameScene::Entities entities;
  GameScene::BackGround bg;

};

