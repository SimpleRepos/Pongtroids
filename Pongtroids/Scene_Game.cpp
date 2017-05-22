#include "Scene_Game.h"
#include "st_SharedState.h"
#include "cl_GfxFactory.h"
#include <vector>
#include <cmath>
#include <fstream>

namespace {
  struct Pos4Norm4Tex2 {
    DirectX::XMFLOAT4 pos;
    DirectX::XMFLOAT4 norm;
    DirectX::XMFLOAT2 texcoord;
  };

  std::vector<Pos4Norm4Tex2> goodVerts;
  std::vector<UINT> goodIndices;
  bool loaded = false;

  void loadMesh() { //pull this shit out into a mesh class
    std::ifstream file("../Assets/asteroid.mesh", std::ios::binary);
    if(!file) { throw std::runtime_error("File not found?"); }

    constexpr unsigned MAGIC_WORD = 'HSEM';

    #pragma pack(1)
    struct MeshFileHead {
      unsigned magic;
      unsigned reserved; //for alignment during inspection in hex editor
      unsigned vertCt;
      unsigned indexCt;
      char textureFileName[16];
      char techniqueName[16];
      struct Material {
        float ambient[3];
        float diffuse[3];
        float specular[3];
        float power;
      } material;
    } header;

    file.read((char*)&header, sizeof(header));
    if(header.magic != MAGIC_WORD) { throw std::runtime_error("Bad mesh file"); }

    #pragma pack(1)
    struct TempVert {
      float pos[3];
      float norm[3];
      float uv[2];
    };

    std::vector<TempVert> verts(header.vertCt);
    file.read((char*)verts.data(), sizeof(TempVert) * verts.size());

    std::vector<short> indices(header.indexCt);
    file.read((char*)indices.data(), sizeof(short) * indices.size());

    for(auto& v : verts) {
      goodVerts.emplace_back(Pos4Norm4Tex2{ DirectX::XMFLOAT4(v.pos), DirectX::XMFLOAT4(v.norm), DirectX::XMFLOAT2(v.uv) });
      goodVerts.back().norm.w = 0;
      goodVerts.back().pos.w = 1;
    }

    for(auto i : indices) { goodIndices.push_back((UINT)i); }

    loaded = true;

  }

  std::vector<Pos4Norm4Tex2> getRoidVerts() {
    if(!loaded) { loadMesh(); }
    return goodVerts;
  }

  std::vector<UINT> getRoidIndices() {
    if(!loaded) { loadMesh(); }
    return goodIndices;
  }

}

Scene_Game::Scene_Game(SharedState& shared) : 
  Scene(shared),
  vShader(shared.factory.createVShader("../Debug/VertexShader.cso", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)),
  pShader(shared.factory.createPShader("../Debug/PixelShader.cso")),
  verts(shared.factory.createStaticVertexBuffer(getRoidVerts())),
  iBuffer(shared.factory.createIndexBuffer(getRoidIndices())),
  tex(shared.factory.createTexture(L"../Assets/asteroid_diffuse.png")),
  cBuffer(shared.factory.createConstantBuffer<DirectX::XMFLOAT4X4>())
{
  shared.win.addKeyFunc(VK_ESCAPE, [](HWND, LPARAM) { PostQuitMessage(0); });

  vShader.set();
  pShader.set();
  verts.set(0);
  iBuffer.set();
  tex.set(0);
  cBuffer.set(0);

  cam.setAspectRatio((float)shared.gfx.VIEWPORT_DIMS.width, (float)shared.gfx.VIEWPORT_DIMS.height);
  cam.setEyePos(0, 0, -5);
  cam.setTargetDir(0, 0, 1);

}

Scene* Scene_Game::update() {
  xform.rotate(DirectX::XMFLOAT3(0, 0, 1), (float)shared.timer.getTickDT());
  xform.rotate(DirectX::XMFLOAT3(0.01f, 0, 0), (float)shared.timer.getTickDT());
  xform.rotate(DirectX::XMFLOAT3(0, 0.1f, 0), (float)shared.timer.getTickDT());
  return this;
}

void Scene_Game::draw() {
  shared.gfx.clear(ColorF::MAGENTA);

  cBuffer.object = cam.getTransposedWVP(xform.asMatrix());
  cBuffer.update();
  shared.gfx.drawIndexed(iBuffer.SIZE);

}
