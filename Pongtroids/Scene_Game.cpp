#include "Scene_Game.h"
#include "st_SharedState.h"
#include "cl_GfxFactory.h"
#include <vector>
#include <cmath>
#include <fstream>
#include "ns_Vertex.h"

Scene_Game::Scene_Game(SharedState& shared) : 
  Scene(shared),
  vShader(shared.factory.createVShader("../Debug/VertexShader.cso", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)),
  pShader(shared.factory.createPShader("../Debug/PixelShader.cso")),
  tex(shared.factory.createTexture(L"../Assets/asteroid_diffuse.png")),
  cBuffer(shared.factory.createConstantBuffer<DirectX::XMFLOAT4X4>()),
  asteroid(shared.factory.createStaticMesh("../Assets/asteroid.mesh", &shared.gfx))
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
  xform.rotate(DirectX::XMFLOAT3(1, 1, 1), (float)shared.timer.getTickDT());
  return this;
}

void Scene_Game::draw() {
  shared.gfx.clear(ColorF::MAGENTA);

  cBuffer.object = cam.getTransposedWVP(xform.asMatrix());
  cBuffer.update();
  asteroid.draw();

}
