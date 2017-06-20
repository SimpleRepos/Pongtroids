#pragma once
#include "vcl_Scene.h"
#include "st_SharedState.h"
#include "st_Asteroids.h"
#include "st_Paddles.h"
#include "st_Ball.h"
#include "tst_RenderProgram.h"
#include "cl_Camera.h"
#include "cl_Texture.h"
#include <DirectXMath.h>
#include "cl_StaticVertexBuffer.h"
#include "cl_GameBackground.h"

class Scene_Game : public Scene {
public:
  Scene_Game(SharedState& shared);
  void passiveUpdate() override;
  Scene* activeUpdate() override;
  void passiveDraw() override;
  void activeDraw() override;

private:
  void ballVPaddles();
  void ballVRoids();
  Scene* processWinLoss();

  RenderProgram<DirectX::XMFLOAT4X4> spriteProg;
  Camera cam;

  GameBackGround bg;
  Asteroids asteroids;
  Paddles paddles;
  Ball ball;

  const float LEFT_OOB;
  const float RIGHT_OOB;

  bool reset;
};

