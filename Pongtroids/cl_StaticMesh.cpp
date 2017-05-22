#include "cl_StaticMesh.h"
#include "cl_GfxFactory.h"
#include "cl_Graphics.h"

void StaticMesh::draw() {
  vBuffer.set(0);

  if(iBuffer.SIZE > 0) {
    iBuffer.set();
    gfx.drawIndexed(iBuffer.SIZE);
  }
  else {
    gfx.draw(vBuffer.SIZE);
  }

}

StaticMesh::StaticMesh(const std::vector<Vertex::Pos3Norm3Tex2>& verts, const std::vector<UINT>& indices, GfxFactory* factory, Graphics* gfx) :
  vBuffer(factory->createStaticVertexBuffer(verts)),
  iBuffer(factory->createIndexBuffer(indices)),
  gfx(*gfx)
{
  //nop
}

