#pragma once
#include <DirectXMath.h>
#include "cl_Transform.h"
#include "simpleCollision.h"
#include "cl_Texture.h"
#include "st_SharedState.h"
#include "tst_RenderProgram.h"

class Camera;

class Ball {
public:
  Ball(SharedState& shared, RenderProgram<DirectX::XMFLOAT4X4>& spriteProg, DirectX::XMFLOAT2 startPos);
  void update(float dt);
  void draw(Camera& cam);

  void setDirection(const DirectX::XMFLOAT2& dir);
  void reset();

  const SC::Circle& getCollider() const { return collider; }
  const DirectX::XMFLOAT2& getVelocity() const { return velocity; }

  static constexpr float RADIUS = 5;
  static constexpr float SPEED = 200;

private:
  SharedState& shared;

  Transform xform;
  const DirectX::XMFLOAT2 resetPos;
  DirectX::XMFLOAT2 velocity;
  SC::Circle collider;

  Texture tex;
  RenderProgram<DirectX::XMFLOAT4X4>* prog;

};

