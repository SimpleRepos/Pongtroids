#pragma once
#include <DirectXMath.h>

///<summary>Static storage for various vertex types</summary>
namespace Vertex {
  struct Pos4Nrm4Tex2 {
    DirectX::XMFLOAT4 pos;
    DirectX::XMFLOAT4 normal;
    DirectX::XMFLOAT2 texcoord;
  };

}
