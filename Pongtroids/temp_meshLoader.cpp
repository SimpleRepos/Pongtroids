#include <fstream>

struct TempMesh {
#pragma pack(1)
  struct TempVert {
    float pos[3];
    float norm[3];
    float uv[2];
  } verts[0x11];
  short indices[0x5A];

  TempMesh() {
    std::ifstream file("../Assets/asteroid.mesh", std::ios::binary);
    if(!file) { throw std::runtime_error("File not found?"); }

    file.seekg(0x58);
    file.read((char*)verts, sizeof(verts));
    file.read((char*)indices, sizeof(indices));
  }


};


