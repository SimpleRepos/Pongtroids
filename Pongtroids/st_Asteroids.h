#pragma once
#include "st_SharedState.h"
#include <DirectXMath.h>
#include "cl_Transform.h"
#include "simpleCollision.h"
#include <vector>
#include "cl_StaticMesh.h"
#include "cl_Texture.h"

namespace GameScene {
  struct Regions;
  struct RenderProgram;
  struct Entities;
}

class Asteroids {
public:
  Asteroids(SharedState& shared, const GameScene::Regions& regions, size_t count);
  void update(float dt, const GameScene::Regions& regions);
  void draw(GameScene::RenderProgram& prog);

  struct Asteroid {
    Asteroid(float size, DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 direction);

    static constexpr float SPEED = 50;
    const float SIZE;
    Transform xform;
    DirectX::XMFLOAT2 velocity;
    bool alive;
    SC::Circle collider;
  };

  std::vector<Asteroid> asteroids;

private:
  SharedState& shared;

  StaticMesh mesh;
  Texture tex;

};
