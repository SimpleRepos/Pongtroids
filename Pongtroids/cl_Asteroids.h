#pragma once
#include "st_SharedState.h"
#include <DirectXMath.h>
#include "cl_Transform.h"
#include "simpleCollision.h"
#include <vector>
#include "cl_StaticMesh.h"
#include "cl_Texture.h"
#include "tst_RenderProgram.h"
#include "cl_Sound.h"

class Camera;

class Asteroids {
  struct RoidArgs;
public:
  Asteroids(SharedState& shared, size_t count);
  void update(float dt);
  void draw(Camera& cam);

  struct Asteroid {
    static constexpr float SPEED = 50;
    static constexpr float MAX_START_SIZE = 75;
    static constexpr float MIN_START_SIZE = 50;
    static constexpr float MIN_SIZE = 13;

    Transform xform;
    DirectX::XMFLOAT2 velocity;
    bool alive;
    SC::Circle collider;
  };

  size_t population() const;
  void hit(Asteroid& roid, DirectX::XMFLOAT2 ballPos);
  Asteroid* findCollision(const SC::Circle& ballCol);

private:
  void enqueue(float size, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT2& dir);
  void clean();
  void spawnQueued();

  static constexpr float HBOUND_MARGIN = 50;

  std::vector<Asteroid> asteroids;
  std::vector<Asteroid> queue;

  SC::Rect centerRegion;

  SharedState& shared;

  Sound hitSound;
  StaticMesh mesh;
  Texture tex;

  RenderProgram<DirectX::XMFLOAT4X4> prog;

};
