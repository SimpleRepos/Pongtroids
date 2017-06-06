#pragma once
#include "vcl_Scene.h"
#include "st_SharedState.h"
#include "ns_GameScene.h"
#include "tst_RenderProgram.h"
#include "cl_Camera.h"
#include "cl_Texture.h"
#include <DirectXMath.h>
#include "cl_StaticVertexBuffer.h"

class Scene_Game : public Scene {
public:
  Scene_Game(SharedState& shared);
  Scene* activeUpdate() override;
  void activeDraw() override;

private:
  const GameScene::Regions regions;

  struct StarCB {
    DirectX::XMFLOAT3 offset;
    float time;
  };

  RenderProgram<DirectX::XMFLOAT4X4> spriteProg;
  RenderProgram<DirectX::XMFLOAT4X4> roidProg;
  RenderProgram<StarCB> starProg;

  Camera cam;

  struct StarVert {
    DirectX::XMFLOAT3 pos;
    float invPeriod;
    DirectX::XMFLOAT3 col;
  };
  static std::vector<StarVert> genStars(SharedState& shared);
  StaticVertexBuffer starVerts;
  static constexpr size_t NUM_STARS = 100;

  GameScene::Entities entities;
  GameScene::BackGround bg;

};

