#pragma once
#include "cl_StaticVertexBuffer.h"
#include "cl_IndexBuffer.h"
#include "ns_Vertex.h"

class Graphics;

class StaticMesh {
public:
  void draw();

private:
  friend class GfxFactory;

  StaticMesh(const std::vector<Vertex::Pos3Norm3Tex2>& verts, const std::vector<UINT>& indices, GfxFactory* factory, Graphics* gfx);

  Graphics& gfx;
  StaticVertexBuffer vBuffer;
  IndexBuffer iBuffer;

};

