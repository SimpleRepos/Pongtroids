#pragma once
#include "vcl_Scene.h"

#include "cl_VertexShader.h"
#include "cl_PixelShader.h"
#include "cl_StaticVertexBuffer.h"
#include "cl_IndexBuffer.h"
#include "cl_Texture.h"
#include "cl_Camera.h"
#include "cl_Transform.h"
#include "tcl_ConstantBuffer.h"

class Scene_Game : public Scene {
public:
  Scene_Game(SharedState& shared);
  Scene* update() override;
  void draw() override;

private:
  VertexShader vShader;
  PixelShader pShader;
  StaticVertexBuffer verts;
  IndexBuffer iBuffer;
  Texture tex;
  Camera cam;
  Transform xform;
  ConstantBuffer<DirectX::XMFLOAT4X4> cBuffer;

};

