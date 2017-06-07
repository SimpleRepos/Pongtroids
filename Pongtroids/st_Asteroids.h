#pragma once
#include "st_SharedState.h"
#include <DirectXMath.h>
#include "cl_Transform.h"
#include "simpleCollision.h"
#include <vector>
#include "cl_StaticMesh.h"
#include "cl_Texture.h"
#include "tst_RenderProgram.h"

namespace GameScene {
  struct Regions;
  struct Entities;
}

class Camera;

class Asteroids {
  struct RoidArgs;
public:
  Asteroids(SharedState& shared, const GameScene::Regions& regions, size_t count);
  void update(float dt, const GameScene::Regions& regions);
  void draw(Camera& cam);

  void enqueue(float size, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT2& dir);
  size_t population() const;

  struct Asteroid {
    Asteroid(const RoidArgs& args);
    void hit(DirectX::XMFLOAT2 ballPos, Asteroids& asteroids);

    static constexpr float SPEED = 50;
    static constexpr float MAX_START_SIZE = 75;
    static constexpr float MIN_START_SIZE = 50;
    static constexpr float MIN_SIZE = 13;

    Transform xform;
    DirectX::XMFLOAT2 velocity;
    bool alive;
    SC::Circle collider;
  };

  std::vector<Asteroid> asteroids;

private:
  void clean();
  void spawnQueued();

  SharedState& shared;

  StaticMesh mesh;
  Texture tex;

  struct RoidArgs {
    float size;
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT2 dir;
  };
  std::vector<RoidArgs> queue;

  RenderProgram<DirectX::XMFLOAT4X4> prog;

};
