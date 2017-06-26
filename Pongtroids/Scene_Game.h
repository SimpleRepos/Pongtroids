#pragma once
#include "vcl_Scene.h"
#include "st_SharedState.h"
#include "cl_Asteroids.h"
#include "cl_Paddles.h"
#include "cl_Ball.h"
#include "tst_RenderProgram.h"
#include "cl_Camera.h"
#include "cl_Texture.h"
#include <DirectXMath.h>
#include "cl_StaticVertexBuffer.h"
#include "cl_GameBackground.h"
#include "cl_ScoreBoard.h"

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
  void ballVBounds();


  RenderProgram<DirectX::XMFLOAT4X4> spriteProg;
  Camera cam;

  GameBackGround bg;
  Asteroids asteroids;
  Paddles paddles;
  Ball ball;
  ScoreBoard scoreBoard;

  const float LEFT_OOB;
  const float RIGHT_OOB;

};

