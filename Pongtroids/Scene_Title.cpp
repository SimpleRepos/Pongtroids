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
  fo = 0;
}

Scene* Scene_Title::activeUpdate() {
  fo += 0.001f;
  return this;
}

void Scene_Title::activeDraw() {
  shared.gfx.clear();

  constexpr float TITLE_Y_POS = 50;
  constexpr float TITLE_SIZE = 64;

  FW1_RECTF rect = titleFont.measure(TITLE, TITLE_SIZE);
  float width = rect.Right - rect.Left;
  float title_x = ((float)shared.gfx.VIEWPORT_DIMS.width - width) / 2;
  title_x -= rect.Left;

  titleFont.drawShadowedText(TITLE, TITLE_SIZE, title_x + fo, TITLE_Y_POS + fo, 2, ColorF::WHITE, ColorF::RED);

  constexpr float APPX_CHAR_WIDTH_FOR_24PT_COURIER_NEW = 44.0f / 3;

  constexpr float ITEM_SIZE = 24;
  constexpr float SPACING = ITEM_SIZE;
  constexpr float TOP_Y = 365;

  float y = TOP_Y;
  for(const auto& item : MENU_OPTIONS) {
    FW1_RECTF rect = menuFont.measure(item, ITEM_SIZE);
    float width = rect.Right - rect.Left;
    float x = ((float)shared.gfx.VIEWPORT_DIMS.width - width) / 2;
    x -= rect.Left;

    menuFont.drawText(item, ITEM_SIZE, x, y, ColorF::WHITE);
    y += SPACING;
  }
}

const std::wstring Scene_Title::TITLE = L"Pongtroids";

const std::array<std::wstring, 4> Scene_Title::MENU_OPTIONS = {
  L"Play",
  L"High Scores",
  L"Credits",
  L"Exit Game"
};
