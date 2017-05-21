#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <atlbase.h>
#include <fstream>
#include "st_Material.h"
#include <vector>

class StaticMesh {
  friend class Scene_Test;
public:
  StaticMesh(const std::string& meshFile);

  StaticMesh(const StaticMesh& other);
  StaticMesh& operator=(const StaticMesh& other);
  StaticMesh(StaticMesh&& other) = delete;
  StaticMesh& operator=(StaticMesh&& other) = delete;

  ~StaticMesh();

  D3DXVECTOR3 position;
  D3DXVECTOR3 rotation;
  float scale;

  bool visible;
private:
  friend class Game;
  friend class Graphics;
  void render();

  struct Vertex {
    D3DXVECTOR3 position;
    D3DXVECTOR3 normal;
    D3DXVECTOR2 texcoord;
  };

  struct MeshFileHead {
    static const unsigned MAGIC_WORD = 'HSEM';
    unsigned magic;
    unsigned reserved; //for alignment during inspection in hex editor
    unsigned vertCt;
    unsigned indexCt;
    char textureFileName[16]; //"assets/" will be prepended
    char techniqueName[16]; //presently this is ignored
    Material::Data matData;
  } data;

  void load(std::istream& input);
  void save(std::ostream& output);

  int handle;
  Material material;
  std::vector<Vertex> verts;
  std::vector<short> indices;
  CComPtr<IDirect3DVertexDeclaration9> vDecl;
  CComPtr<IDirect3DVertexBuffer9> vBuffer;
  CComPtr<IDirect3DIndexBuffer9>  iBuffer;

  static Game* game;
};

