#include "Scene_Game.h"
#include "st_SharedState.h"
#include "cl_GfxFactory.h"
#include <vector>
#include <cmath>
#include <fstream>
#include "ns_Vertex.h"
#include <random>

Scene_Game::Scene_Game(SharedState& shared) : 
  Scene(shared),
  vShader(shared.factory.createVShader("../Assets/VertexShader.cso", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)),
  pShader(shared.factory.createPShader("../Assets/PixelShader.cso")), 
  tex(shared.factory.createTexture(L"../Assets/asteroid_diffuse.png")),
  cBuffer(shared.factory.createConstantBuffer<DirectX::XMFLOAT4X4>()),
  mesh(shared.factory.createStaticMeshFromOldMeshFileFormat("../Assets/asteroid.mesh")),
  roid({0,0}, {0.2f, 0.3f})
{
  shared.win.addKeyFunc(VK_ESCAPE, [](HWND, LPARAM) { PostQuitMessage(0); });

  vShader.set();
  pShader.set();
  tex.set(0);
  cBuffer.set(0);

  cam.setAspectRatio((float)shared.gfx.VIEWPORT_DIMS.width, (float)shared.gfx.VIEWPORT_DIMS.height);
  cam.setEyePos(0, 0, -5);
  cam.setTargetDir(0, 0, 1);
}

Scene* Scene_Game::update() {
  roid.update((float)shared.timer.getTickDT());
  return this;
}

void Scene_Game::draw() {
  shared.gfx.clear(ColorF::MAGENTA);
  cBuffer.object = cam.getTransposedWVP(roid.xform);
  cBuffer.update();
  shared.gfx.draw(mesh);
}
