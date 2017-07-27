#pragma once
#include <DirectXMath.h>
#include "cl_Transform.h"
#include "simpleCollision.h"
#include "cl_Texture.h"
#include "st_SharedState.h"
#include "tst_RenderProgram.h"
#include "cl_Sound.h"

class Camera;

class Ball {
public:
  Ball(SharedState& shared, RenderProgram<DirectX::XMFLOAT4X4>& spriteProg, DirectX::XMFLOAT2 startPos);
  virtual void update(float dt);
  void draw(Camera& cam);

  void deflect(const DirectX::XMFLOAT2& normal);
  void setDirection(const DirectX::XMFLOAT2& dir);
  void reset();

  const SC::Circle& getCollider() const { return collider; }
  const DirectX::XMFLOAT2& getVelocity() const { return velocity; }

  static constexpr float RADIUS = 5;
  static constexpr float SPEED = 200;

protected:
  SharedState& shared;

  Transform xform;
  const DirectX::XMFLOAT2 resetPos;
  DirectX::XMFLOAT2 velocity;
  SC::Circle collider;

  Texture tex;
  RenderProgram<DirectX::XMFLOAT4X4>* prog;

  Sound wallBounce;

};

//ball controlled directly by the mouse for use in deflection debugging
class DebugBall : public Ball {
public:
  DebugBall(SharedState& shared, RenderProgram<DirectX::XMFLOAT4X4>& spriteProg, DirectX::XMFLOAT2 startPos);
  void update(float dt);

private:
  static constexpr float TEST_DURATION = 2.0f;
  float testing;
  bool aiming;
  DirectX::XMFLOAT3 mpos;

};
