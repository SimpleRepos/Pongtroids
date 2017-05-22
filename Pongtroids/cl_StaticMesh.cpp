#include "cl_StaticMesh.h"
#include "cl_GfxFactory.h"

void StaticMesh::set() const {
  vBuffer.set(0);
  if(iBuffer.SIZE > 0) { iBuffer.set(); }
}

StaticMesh::StaticMesh(const std::vector<Vertex::Pos3Norm3Tex2>& verts, const std::vector<UINT>& indices, GfxFactory* factory) :
  vBuffer(factory->createStaticVertexBuffer(verts)),
  iBuffer(factory->createIndexBuffer(indices)),
  SIZE((indices.size() > 0) ? indices.size() : verts.size()),
  INDEXED(indices.size() > 0)
{
  //nop
}

