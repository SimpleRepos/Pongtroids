#pragma once
#include "cl_Transform.h"
#include "simpleCollision.h"
#include "st_SharedState.h"
#include "cl_StaticMesh.h"
#include "cl_Texture.h"

class Camera;

class Paddles {
public:
  Paddles(SharedState& shared);

  enum Side {
    LEFT = 0,
    RIGHT = 1
  };

  void update(float dt);
  void draw(ConstantBuffer<DirectX::XMFLOAT4X4>& cBuffer, const Camera& cam);

  const Transform& getXform(Side side)   const { return xforms[side]; }
  const SC::Rect& getCollider(Side side) const { return colliders[side]; }

private:
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


