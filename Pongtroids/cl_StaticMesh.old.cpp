#include "cl_StaticMesh.h"
#include "cl_Game.h"

namespace {
  std::wstring toWide(const std::string& str) {
    std::wstring derp;
    derp.assign(str.begin(), str.end());
    return derp;
  }

  const D3DVERTEXELEMENT9 vertElements[] = {
    {0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,   0},
    {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,     0},
    {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,   0},
    D3DDECL_END()
  };

}

Game* StaticMesh::game;

StaticMesh::StaticMesh(const std::string& meshFile) : position(0,0,0), rotation(0,0,0), scale(1) {
  if(meshFile.compare("?DEBUG?") != 0) {
    std::ifstream file(meshFile, std::ios::binary|std::ios::in);
    if(!file) {throw std::runtime_error("StaticMesh::StaticMesh() - Failed to open file.");}
    load(file);
  }

  game->gfx.dev->CreateVertexDeclaration(vertElements, &vDecl);

  handle = game->gfx.staticMeshes.push(this);

  visible = true;
}

StaticMesh::StaticMesh(const StaticMesh& other) : handle(Graphics::NO_OBJECT) {
  *this = other;
}

StaticMesh& StaticMesh::operator=(const StaticMesh& other) {
  position = other.position;
  rotation = other.rotation;
  scale = other.scale;
  visible = other.visible;
  data = other.data;
  verts = other.verts;
  indices = other.indices;
  material = other.material;
  vDecl = other.vDecl;
  vBuffer = other.vBuffer;
  iBuffer = other.iBuffer;

  if(handle != Graphics::NO_OBJECT) {game->gfx.staticMeshes.erase(handle);}
  handle = game->gfx.staticMeshes.push(this);

  return *this;
}

StaticMesh::~StaticMesh() {
  game->gfx.staticMeshes.erase(handle);
}

void StaticMesh::render() {
  auto& shader = game->gfx.shader;
  auto& viewMat = game->gfx.viewMat;
  auto& projMat = game->gfx.projMat;

  LPDIRECT3DDEVICE9 dev;
  shader->GetDevice(&dev);
  dev->SetVertexDeclaration(vDecl);

  D3DXMATRIX transMat, rotMat, scaleMat;
  D3DXMatrixTranslation(&transMat, position.x, position.y, position.z);
  
  D3DXQUATERNION qx, qy, qz, quat;
  D3DXQuaternionRotationAxis(&qx, &D3DXVECTOR3(1,0,0), rotation.x);
  D3DXQuaternionRotationAxis(&qy, &D3DXVECTOR3(0,1,0), rotation.y);
  D3DXQuaternionRotationAxis(&qz, &D3DXVECTOR3(0,0,1), rotation.z);
  D3DXQuaternionNormalize(&quat, &(qx * qy * qz));
  D3DXMatrixRotationQuaternion(&rotMat, &quat);

  D3DXMatrixScaling(&scaleMat, scale, scale, scale);
  D3DXMATRIX world = rotMat * scaleMat * transMat;
  shader->SetMatrix("worldMat", &world);
      
  D3DXMATRIX worldViewProjMat = world * viewMat * projMat;
  shader->SetMatrix("worldViewProjMat", &worldViewProjMat);

  D3DXMATRIX worldInverseTransposeMat;
  D3DXMatrixInverse(&worldInverseTransposeMat, 0, &world);
  D3DXMatrixTranspose(&worldInverseTransposeMat, &worldInverseTransposeMat);
  shader->SetMatrix("worldInverseTransposeMat", &worldInverseTransposeMat);

  shader->SetValue("material", &material.data, sizeof(material.data));

  if(material.texture) {
    shader->SetBool("useTex", TRUE);
    shader->SetTexture("tex", game->gfx.textures[material.texture->handle]);
  }
  else {
    shader->SetBool("useTex", FALSE);
  }

  shader->CommitChanges();
  
  dev->SetStreamSource(0, vBuffer, 0, sizeof(Vertex));
  dev->SetIndices(iBuffer);
  dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, verts.size(), 0, indices.size() / 3);
  dev->Release();
}

void StaticMesh::load(std::istream& input) {
  //read information from file
  input.read((char*)&data, sizeof(data));
  if(!input) {throw std::runtime_error("StaticMesh::load() - Failed to read header data.");}
  if(data.magic != data.MAGIC_WORD) {throw std::runtime_error("StaticMesh::load() - Not a valid mesh file.");}
  
  //read vertices from file
  verts.resize(data.vertCt);
  input.read((char*)verts.data(), sizeof(Vertex) * data.vertCt);
  if(!input) {throw std::runtime_error("StaticMesh::load() - Failed to read vertex data.");}

  //read indices from file
  indices.resize(data.indexCt);
  input.read((char*)indices.data(), sizeof(short) * data.indexCt);
  if(!input) {throw std::runtime_error("StaticMesh::load() - Failed to read index data.");}

  //prepare material
  material.data = data.matData;
  if(data.textureFileName[0]) {
    std::wstring fname = L"assets/";
    fname.append(toWide(data.textureFileName));
    material.texture.reset(new Texture(fname));
  }
  material.technique = game->gfx.shader->GetTechniqueByName(data.techniqueName);
  if(!material.technique) {throw std::runtime_error("StaticMesh.load() - Matching technique not found in shader.");}

  //upload geometry
  void* vptr = nullptr;
  UINT vsz = verts.size() * sizeof(Vertex);
  HRESULT hr = game->gfx.dev->CreateVertexBuffer(vsz, 0, 0, D3DPOOL_MANAGED, &vBuffer, 0);
  checkResult(hr, "StaticMesh::load() - Failed to create vertex buffer.");
  hr = vBuffer->Lock(0, vsz, &vptr, 0);
  checkResult(hr, "StaticMesh::load() - Failed to lock vertex buffer.");
  memcpy_s(vptr, vsz, verts.data(), vsz);
  vBuffer->Unlock();

  //upload indices
  UINT isz = indices.size() * sizeof(short);
  hr = game->gfx.dev->CreateIndexBuffer(isz, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &iBuffer, 0);
  hr = iBuffer->Lock(0, isz, &vptr, 0);
  memcpy_s(vptr, isz, indices.data(), isz);
  iBuffer->Unlock();
}

void StaticMesh::save(std::ostream& output) {
  output.write((char*)&data, sizeof(data));
  if(!output) {throw std::runtime_error("StaticMesh::save() - Failed to write header data.");}
  output.write((char*)verts.data(), sizeof(Vertex) * verts.size());
  if(!output) {throw std::runtime_error("StaticMesh::save() - Failed to write vertex data.");}
  output.write((char*)indices.data(), sizeof(short) * indices.size());
  if(!output) {throw std::runtime_error("StaticMesh::save() - Failed to write index data.");}
}

