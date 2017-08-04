#include "Scene_Title.h"

Scene_Title::Scene_Title(SharedState& shared) : 
  Scene(shared),
  spriteProg{
    ShaderSet(
      shared.factory,
      "../Assets/vs_sprite.cso", D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
      "", "", "",
      RasterizerState::DEFAULT_DESC,
      "../Assets/ps_texture.cso"
    ),
    shared.factory.createConstantBuffer<DirectX::XMFLOAT4X4>()
  },
  titleFont(shared.factory.createFont(L"Arial")),
  menuFont(shared.factory.createFont(L"Courier New"))
{
  //~~_
}

void Scene_Title::passiveUpdate() {
  //~~_
}

Scene* Scene_Title::activeUpdate() {
  return this;
}

void Scene_Title::passiveDraw() {
  constexpr float TITLE_WIDTH_AT_64PT = 298;
  float title_x = ((float)shared.gfx.VIEWPORT_DIMS.width - TITLE_WIDTH_AT_64PT) / 2;
  titleFont.drawText(TITLE, 64, title_x, 50, ColorF::WHITE);

  constexpr float APPX_CHAR_WIDTH_FOR_24PT_COURIER_NEW = 44.0f / 3;

  float y = 365;
  for(const auto& item : MENU_OPTIONS) {
    float width = item.size() * APPX_CHAR_WIDTH_FOR_24PT_COURIER_NEW;
    float x = ((float)shared.gfx.VIEWPORT_DIMS.width - width) / 2;

    menuFont.drawText(item, 24, x, y, ColorF::WHITE);
    y += 24;
  }
}

void Scene_Title::activeDraw() {
  //~~_
}

const std::wstring Scene_Title::TITLE = L"Pongtroids";

const std::array<std::wstring, 4> Scene_Title::MENU_OPTIONS = {
  L"Play",
  L"High Scores",
  L"Credits",
  L"Exit Game"
};
