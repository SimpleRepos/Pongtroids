#include "Scene_Title.h"
#include "GDIFontTool.h"

const std::wstring Scene_Title::TITLE = L"Pongtroids";

const std::wstring Scene_Title::MENU_OPTIONS = {
  L"Play\n"
  L"High Scores\n"
  L"Credits\n"
  L"Exit Game"
};

Scene_Title::Scene_Title(SharedState& shared) : 
  Scene(shared),
  fontProg{
    ShaderSet(
      shared.factory,
      "../Assets/vs_sprite.cso", D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
      "", "", "",
      RasterizerState::DEFAULT_DESC,
      "../Assets/ps_font.cso"
    ),
    shared.factory.createConstantBuffer<CBuffer>()
  }
{
  D3D11_RENDER_TARGET_BLEND_DESC desc = {
    TRUE, //enable
    D3D11_BLEND_SRC_ALPHA, //src
    D3D11_BLEND_INV_SRC_ALPHA, //dest
    D3D11_BLEND_OP_ADD, //op
    D3D11_BLEND_ONE, //srca
    D3D11_BLEND_ONE, //desta
    D3D11_BLEND_OP_ADD, //opa
    D3D11_COLOR_WRITE_ENABLE_ALL //write mask
  };
  shared.gfx.setBlend(desc);

  cam.setDepthLimits(0, 100);
  cam.setEyePos(0, 0, -1);
  cam.setTargetPos(0, 0, 0);
  cam.setOrthographic(800, 600);

  GDIFontTool::Factory fac(shared.win.getHandle());
  GDIFontTool::Font titleFont = fac.createFont(L"Arial", 64);
  GDIFontTool::Font menuFont = fac.createFont(L"Courier New", 24);

  auto rawTitle = titleFont.print(TITLE, -2, 2);
  title = std::make_unique<Texture>(shared.factory.createTexture(DirectX::XMUINT2(rawTitle.width, rawTitle.height), rawTitle.data));

  auto rawMenu = menuFont.print(MENU_OPTIONS, 1);
  menu = std::make_unique<Texture>(shared.factory.createTexture(DirectX::XMUINT2(rawMenu.width, rawMenu.height), rawMenu.data));

  titleXF.scale = DirectX::XMFLOAT3((float)title->dims.x, (float)title->dims.y, 0);
  titleXF.translation = DirectX::XMFLOAT3(((float)shared.win.USER_AREA_DIMS.width  - titleXF.scale.x) / 2, 50, 0);

  menuXF.scale = DirectX::XMFLOAT3((float)menu->dims.x, (float)menu->dims.y, 0);
  menuXF.translation = DirectX::XMFLOAT3(((float)shared.win.USER_AREA_DIMS.width  - menuXF.scale.x) / 2, 365, 0);

}

Scene* Scene_Title::activeUpdate() {
  return this;
}

void Scene_Title::activeDraw() {
  shared.gfx.clear();

  title->set(0);
  fontProg.cBuffer.object.foreColor = DirectX::XMFLOAT4(ColorF::RED);
  fontProg.cBuffer.object.backColor = DirectX::XMFLOAT4(ColorF::GREEN);
  fontProg.cBuffer.object.transform = cam.getTransposedWVP(titleXF);
  fontProg.cBuffer.update();
  fontProg.set();
  shared.gfx.draw(4);

  menu->set(0);
  fontProg.cBuffer.object.foreColor = DirectX::XMFLOAT4(ColorF::WHITE);
  fontProg.cBuffer.object.backColor = DirectX::XMFLOAT4(ColorF::BLUE);
  fontProg.cBuffer.object.transform = cam.getTransposedWVP(menuXF);
  fontProg.cBuffer.update();
  fontProg.set();
  shared.gfx.draw(4);

}
