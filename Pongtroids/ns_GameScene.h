#pragma once
#include "st_SharedState.h"
#include "simpleCollision.h"
#include "st_ShaderSet.h"
#include "cl_Camera.h"
#include "tcl_ConstantBuffer.h"
#include "cl_StaticMesh.h"
#include "st_Asteroids.h"
#include "st_Paddles.h"
#include "st_Ball.h"

namespace GameScene {
  struct Regions {
    Regions(SharedState& shared);

    SC::Rect middle;
    SC::Rect left;
    SC::Rect top;
    SC::Rect right;
    SC::Rect bottom;
  };

  struct RenderProgram {
    RenderProgram(SharedState& shared);
    void set();

    ShaderSet shaders;
    Camera cam;
    ConstantBuffer<DirectX::XMFLOAT4X4> cBuffer;

    StaticMesh squareMesh;
  };

  struct Entities {
    Entities(SharedState& shared, const Regions& regions, size_t numRoids);
    void update(float dt, const GameScene::Regions& regions);
    void draw(RenderProgram& prog);

    Asteroids asteroids;
    Paddles paddles;
    Ball ball;
  };

  struct BackGround {
    BackGround(SharedState& shared, RenderProgram& prog, const Regions& regions);
    void draw(RenderProgram& prog);

    SharedState& shared;
    Texture tex;
    DirectX::XMFLOAT4X4 bgx;
  };

}
