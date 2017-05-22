#pragma once
#include "cl_StaticVertexBuffer.h"
#include "cl_IndexBuffer.h"
#include "ns_Vertex.h"

class StaticMesh {
public:
  const size_t SIZE;
  const bool INDEXED;

  void set() const;

private:
  friend class GfxFactory;
  friend class Graphics;

  StaticMesh(const std::vector<Vertex::Pos3Norm3Tex2>& verts, const std::vector<UINT>& indices, GfxFactory* factory);

  StaticVertexBuffer vBuffer;
  IndexBuffer iBuffer;

};

