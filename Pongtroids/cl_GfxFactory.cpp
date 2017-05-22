#include "cl_GfxFactory.h"
#include "FW1FontWrapper_1_1/FW1FontWrapper.h"
#include "cl_ImageLoader.h"
#include "cl_IndexBuffer.h"
#include "tcl_DynamicVertexBuffer.h"
#include "cl_VertexShader.h"
#include "cl_GeometryShader.h"
#include "cl_HullShader.h"
#include "cl_DomainShader.h"
#include "cl_PixelShader.h"
#include "cl_Texture.h"
#include "cl_StaticVertexBuffer.h"
#include "cl_RasterizerState.h"
#include "cl_StaticMesh.h"
#include <fstream>
#include "ns_Vertex.h"

GfxFactory::GfxFactory(ID3D11Device* device, ID3D11DeviceContext* context) :
  imageLoader(std::make_unique<ImageLoader>()),
  device(device),
  context(context)
{
  HR(FW1CreateFactory(FW1_VERSION, &fontFactory));
}

GfxFactory::~GfxFactory() {}

Font GfxFactory::createFont(const std::wstring& fontFace) {
  return Font(fontFactory, device, context, fontFace);
}

IndexBuffer GfxFactory::createIndexBuffer(const std::vector<UINT>& indices) {
  return IndexBuffer(indices, device.p, context.p);
}

VertexShader GfxFactory::createVShader(const std::string& filename, D3D11_PRIMITIVE_TOPOLOGY topology) {
  return VertexShader(filename, topology, device.p, context.p);
}

HullShader GfxFactory::createHShader(const std::string& filename) {
  return HullShader(filename, device.p, context.p);
}

DomainShader GfxFactory::createDShader(const std::string& filename) {
  return DomainShader(filename, device.p, context.p);
}

GeometryShader GfxFactory::createGShader(const std::string& filename) {
  return GeometryShader(filename, device.p, context.p);
}

PixelShader GfxFactory::createPShader(const std::string& filename) {
  return PixelShader(filename, device.p, context.p);
}

RasterizerState GfxFactory::createRasterizerState(const D3D11_RASTERIZER_DESC& desc) {
  return RasterizerState(device.p, context.p, desc);
}

Texture GfxFactory::createTexture(const std::wstring& filename, D3D11_FILTER filter) {
  return Texture(device.p, context.p, imageLoader->load(filename, device.p), filter);
}

Texture GfxFactory::createTexture(const DirectX::XMUINT2 dimensions, const std::vector<ColorF::uCol32>& data, D3D11_FILTER filter) {
  return Texture(device.p, context.p, imageLoader->create(dimensions, data, device.p), filter);
}

Texture GfxFactory::createTexture(const DirectX::XMUINT2 dimensions, const std::vector<uint8_t>& data, D3D11_FILTER filter) {
  return Texture(device.p, context.p, imageLoader->create(dimensions, data, device.p), filter);
}

std::pair<DirectX::XMUINT2, std::vector<ColorF::uCol32>> GfxFactory::loadRawTextureData(const std::wstring& filename) {
  return imageLoader->loadRaw(filename);
}

StaticMesh GfxFactory::createStaticMesh(const std::string& filename, Graphics* gfx) {
  std::ifstream file(filename, std::ios::binary);
  if(!file) { throw std::runtime_error("Failed to open mesh file."); }

  constexpr unsigned MAGIC_WORD = 'HSEM';

  struct {
    unsigned magic;
    unsigned reserved;
    unsigned vertCt;
    unsigned indexCt;
    unsigned unused[18];
  } header;

  file.read((char*)&header, sizeof(header));
  if(header.magic != MAGIC_WORD) { throw std::runtime_error("Bad mesh file"); }

  std::vector<Vertex::Pos3Norm3Tex2> verts(header.vertCt);
  file.read((char*)verts.data(), sizeof(Vertex::Pos3Norm3Tex2) * verts.size());

  std::vector<short> shortIndices(header.indexCt);
  file.read((char*)shortIndices.data(), sizeof(short) * shortIndices.size());

  std::vector<UINT> indices(shortIndices.size());
  for(auto i : shortIndices) { indices.push_back((UINT)i); }

  return StaticMesh(verts, indices, this, gfx);

}


