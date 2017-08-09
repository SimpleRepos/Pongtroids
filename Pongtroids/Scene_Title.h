#pragma once
#include "vcl_Scene.h"
#include "st_SharedState.h"
#include "tst_RenderProgram.h"
#include "cl_Camera.h"
#include "cl_Font.h"
#include <array>
#include <string>

class Scene_Title : public Scene {
public:
  Scene_Title(SharedState& shared);
  Scene* activeUpdate() override;
  void activeDraw() override;

private:
  static const std::wstring TITLE;
  static const std::array<std::wstring, 4> MENU_OPTIONS;

  RenderProgram<DirectX::XMFLOAT4X4> spriteProg;
  Camera cam;
  Font titleFont;
  Font menuFont;

};


