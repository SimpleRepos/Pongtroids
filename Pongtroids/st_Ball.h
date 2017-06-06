#pragma once
#include <DirectXMath.h>
#include "cl_Transform.h"
#include "simpleCollision.h"
#include "cl_Texture.h"
#include "st_SharedState.h"
#include "tst_RenderProgram.h"

namespace GameScene {
  struct Regions;
  struct Entities;
}

class Graphics;
class Camera;

struct Ball {
  Ball(SharedState& shared, DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 velocity);
  void update(float dt, GameScene::Entities& entities, const GameScene::Regions& regions);
  void draw(RenderProgram<DirectX::XMFLOAT4X4>& prog, Camera& cam);

  static constexpr float RADIUS = 5;
  static constexpr float SPEED = 200;

  SharedState& shared;

  Transform xform;
  DirectX::XMFLOAT2 velocity;
  SC::Circle collider;

  Texture tex;

};

