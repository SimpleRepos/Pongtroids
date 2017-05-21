#include "Scene_Game.h"
#include "st_SharedState.h"
#include "cl_GfxFactory.h"
#include <vector>
#include <cmath>

namespace {
  struct Pos4Tex2 {
    DirectX::XMFLOAT4 pos;
    DirectX::XMFLOAT2 texcoord;
  };

  std::vector<Pos4Tex2> generateVerts(int ct) {
    float radsPerVert = DirectX::XM_2PI / ct;

    std::vector<Pos4Tex2> vv;
    for(float rads = 0; rads < DirectX::XM_2PI; rads += radsPerVert) {
      vv.emplace_back(Pos4Tex2{ { 0, 0, 0, 1 }, { 0, 0 } });

      float x = std::sin(rads);
      float y = std::cos(rads);
      vv.emplace_back(Pos4Tex2{ { x, y, 0, 1 }, { x, y } });
    }

    vv.emplace_back(Pos4Tex2{ { 0, 0, 0, 1 }, { 0, 0 } });
    vv.emplace_back(vv[1]);

    return vv;
  }

}

Scene_Game::Scene_Game(SharedState& shared) : 
  Scene(shared),
  vShader(shared.factory.createVShader("../Debug/VertexShader.cso", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP)),
  pShader(shared.factory.createPShader("../Debug/PixelShader.cso")),
  verts(shared.factory.createStaticVertexBuffer(generateVerts(100))),
  tex(shared.factory.createTexture(L"../Assets/checkers.png")),
  cBuffer(shared.factory.createConstantBuffer<CB>())
{
  shared.win.addKeyFunc(VK_ESCAPE, [](HWND, LPARAM) { PostQuitMessage(0); });

  vShader.set();
  pShader.set();
  verts.set(0);
  tex.set(0);
  cBuffer.set(0);

  cam.setAspectRatio((float)shared.gfx.VIEWPORT_DIMS.width, (float)shared.gfx.VIEWPORT_DIMS.height);
  cam.setEyePos(0, 0, -5);
  cam.setTargetDir(0, 0, 1);

}

Scene* Scene_Game::update() {
  xform.rotate(DirectX::XMFLOAT3(0, 0, 1), (float)shared.timer.getTickDT());
  return this;
}

void Scene_Game::draw() {
  shared.gfx.clear(ColorF::MAGENTA);

  cBuffer.object.transWVP = cam.getTransposedWVP(xform.asMatrix());
  cBuffer.update();
  shared.gfx.draw(verts.SIZE);

}
