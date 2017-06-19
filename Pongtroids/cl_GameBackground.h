#pragma once
#include "st_SharedState.h"
#include "tst_RenderProgram.h"
#include <DirectXMath.h>
#include "cl_Texture.h"
#include "cl_Transform.h"
#include <vector>
#include "cl_StaticVertexBuffer.h"

class Camera;

class GameBackGround {
public:
  GameBackGround(SharedState& shared, RenderProgram<DirectX::XMFLOAT4X4>& spriteProg);
  void update(float dt);
  void draw(Camera& cam);

private:
  SharedState& shared;
  Texture tex;
  Transform xform;
  RenderProgram<DirectX::XMFLOAT4X4>* spriteProg;

  struct StarCB {
    DirectX::XMFLOAT3 offset;
    float time;
  };
  RenderProgram<StarCB> starProg;

  struct StarVert {
    DirectX::XMFLOAT3 pos;
    float invPeriod;
    DirectX::XMFLOAT3 col;
  };
  static std::vector<StarVert> genStars(SharedState& shared);
  StaticVertexBuffer starVerts;
  static constexpr size_t NUM_STARS = 100;

};
