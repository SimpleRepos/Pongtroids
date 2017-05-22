#pragma once
#include <DirectXMath.h>

namespace Vertex {
  struct Pos3Norm3Tex2 {
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT3 norm;
    DirectX::XMFLOAT2 texcoord;
  };
}

