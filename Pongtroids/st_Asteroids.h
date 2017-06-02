#pragma once
#include "cl_Transform.h"
#include "simpleCollision.h"
#include "cl_StaticMesh.h"
#include "cl_Texture.h"
#include "st_SharedState.h"
#include "cl_Camera.h"
#include <vector>

struct Regions;

class Asteroids {
public:
  Asteroids(SharedState& shared, const Regions& regions, size_t count);
  void update(float dt);
  void draw(ConstantBuffer<DirectX::XMFLOAT4X4>& cBuffer, const Camera& cam);

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
  const Regions& regions;

  StaticMesh mesh;
  Texture tex;

};
