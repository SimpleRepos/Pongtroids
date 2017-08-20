#pragma once
#include "vcl_Scene.h"
#include "st_SharedState.h"
#include "tst_RenderProgram.h"
#include "cl_Camera.h"
#include <array>
#include <string>
#include "cl_Texture.h"
#include <memory>
#include "cl_Transform.h"

class Scene_Title : public Scene {
public:
  Scene_Title(SharedState& shared);
  Scene* activeUpdate() override;
  void activeDraw() override;

private:
  static const std::wstring TITLE;
  static const std::wstring MENU_OPTIONS;

  struct CBuffer {
    DirectX::XMFLOAT4X4 transform;
    DirectX::XMFLOAT4   foreColor;
    DirectX::XMFLOAT4   backColor;
  };

  RenderProgram<CBuffer> fontProg;

  std::unique_ptr<Texture> title;
  std::unique_ptr<Texture> menu;

  Camera cam;
  Transform titleXF;
  Transform menuXF;

};


