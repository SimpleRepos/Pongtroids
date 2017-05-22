#pragma once
#include <DirectXMath.h>

struct Transform {
  void translate(DirectX::XMFLOAT3 offset);
  void rotate(DirectX::XMFLOAT3 axis, float radians);
  void mulScale(DirectX::XMFLOAT3 factors);

  DirectX::XMFLOAT3 translation = { 0, 0, 0 };
  DirectX::XMFLOAT4 rotationQuaternion = { 0, 0, 0, 1 };
  DirectX::XMFLOAT3 scale = { 1, 1, 1 };

  operator DirectX::XMFLOAT4X4() const;

};

