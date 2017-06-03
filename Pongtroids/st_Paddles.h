#pragma once
#include "st_SharedState.h"
#include "cl_Transform.h"
#include "simpleCollision.h"
#include "cl_StaticMesh.h"
#include "cl_Texture.h"

namespace GameScene {
  struct Regions;
  struct RenderProgram;
  struct Entities;
}

class Paddles {
public:
  Paddles(SharedState& shared);
  void update(float dt);
  void draw(GameScene::RenderProgram& prog);

  enum Side {
    LEFT = 0,
    RIGHT = 1
  };

  const Transform& getXform(Side side)   const { return xforms[side]; }
  const SC::Rect& getCollider(Side side) const { return colliders[side]; }

  float getDeflectionAngle(Side side, float ballY) const;

private:
  static constexpr float MAX_DEFLECTION_ANGLE = 0.785398163f;
  static constexpr float HORZ_BORDER_GAP = 15;
  static constexpr float WIDTH = 20;
  static constexpr float HEIGHT = 100;
  static constexpr float SPEED = 200;

  const float TOP_BOUND;
  const float BOT_BOUND;

  Transform xforms[2];
  SC::Rect colliders[2];
  SharedState& shared;

  StaticMesh mesh;
  Texture tex;

};


