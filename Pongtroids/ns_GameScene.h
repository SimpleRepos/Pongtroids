#pragma once
#include "st_SharedState.h"
#include "simpleCollision.h"
#include "st_ShaderSet.h"
#include "tcl_ConstantBuffer.h"
#include "cl_StaticMesh.h"
#include "st_Asteroids.h"
#include "st_Paddles.h"
#include "st_Ball.h"
#include "tst_RenderProgram.h"

class Camera;

namespace GameScene {
  struct Regions {
    Regions(SharedState& shared);

    SC::Rect middle;
    SC::Rect left;
    SC::Rect top;
    SC::Rect right;
    SC::Rect bottom;
  };

  StaticMesh genSquareMesh(SharedState& shared);

  struct Entities {
    Entities(SharedState& shared, const Regions& regions, size_t numRoids);
    void update(float dt, const GameScene::Regions& regions);
    void draw(RenderProgram<DirectX::XMFLOAT4X4>& prog, Camera& cam);

    Asteroids asteroids;
    Paddles paddles;
    Ball ball;
  };

  struct BackGround {
    BackGround(SharedState& shared, const Regions& regions);
    void draw(RenderProgram<DirectX::XMFLOAT4X4>& prog, Camera& cam);

    SharedState& shared;
    Texture tex;
    Transform xform;
    StaticMesh mesh;
  };

}
