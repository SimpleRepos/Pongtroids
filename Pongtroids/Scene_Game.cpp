#include "Scene_Game.h"
#include "st_SharedState.h"
#include "cl_GfxFactory.h"
#include <vector>
#include "ns_Vertex.h"

namespace {
  const std::vector<Vertex::Pos4Nrm4Tex2> triVerts = { //~~! unchecked winding order
    { DirectX::XMFLOAT4( 0.0f, -0.5f, 0, 1), DirectX::XMFLOAT4(0, 0, 1, 0), DirectX::XMFLOAT2(0.5f, 0.0f) },
    { DirectX::XMFLOAT4( 0.5f,  0.5f, 0, 1), DirectX::XMFLOAT4(0, 0, 1, 0), DirectX::XMFLOAT2(1.0f, 1.0f) },
    { DirectX::XMFLOAT4(-0.5f,  0.5f, 0, 1), DirectX::XMFLOAT4(0, 0, 1, 0), DirectX::XMFLOAT2(0.0f, 1.0f) }
  };
}

Scene_Game::Scene_Game(SharedState& shared) : 
  Scene(shared),
  verts(shared.factory.createStaticVertexBuffer(triVerts)),
  tex(shared.factory.createTexture(L"../Assets/checkers.png"))
{
  shared.win.addKeyFunc(VK_ESCAPE, [](HWND, LPARAM) { PostQuitMessage(0); });

  cam.setEyePos(DirectX::XMFLOAT3(0, 0, 5));
  cam.setTargetDir(DirectX::XMFLOAT3(0, 0, -1));

}

Scene* Scene_Game::update() {
  return this;
}

void Scene_Game::draw() {
  shared.gfx.clear();

}
