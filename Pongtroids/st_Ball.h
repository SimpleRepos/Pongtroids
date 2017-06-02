#pragma once
#include <DirectXMath.h>
#include "cl_Transform.h"
#include "simpleCollision.h"
#include "cl_Texture.h"
#include "st_SharedState.h"

namespace GameScene {
  struct Regions;
  struct RenderProgram;
  struct Entities;
}

class Graphics;

struct Ball {
  Ball(SharedState& shared, DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 velocity);
  void update(float dt, GameScene::Entities& entities, const GameScene::Regions& regions);
  void draw(GameScene::RenderProgram& prog);

  static constexpr float RADIUS = 5;
  static constexpr float SPEED = 200;

  SharedState& shared;

  Transform xform;
  DirectX::XMFLOAT2 velocity;
  SC::Circle collider;

  Texture tex;

};

