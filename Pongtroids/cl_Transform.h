#pragma once
#include <DirectXMath.h>

struct Transform {
  Transform(
    DirectX::XMFLOAT3 translation = { 0, 0, 0 },
    DirectX::XMFLOAT3 scale = { 1, 1, 1 },
    DirectX::XMFLOAT4 rotationQuaternion = { 0, 0, 0, 1 }
    );

  Transform(const Transform& other);
  Transform& operator=(const Transform& other);

  void translate(DirectX::XMFLOAT3 offset);
  void translate(DirectX::XMVECTOR& offset);
  void rotate(DirectX::XMFLOAT3 axis, float radians);
  void rotate(DirectX::XMVECTOR& axis, float radians);
  void mulScale(DirectX::XMFLOAT3 factors);
  void mulScale(float uniformFactor);
  operator DirectX::XMFLOAT4X4() const;

  DirectX::XMFLOAT3 translation;
  DirectX::XMFLOAT3 scale;
  DirectX::XMFLOAT4 rotationQuaternion;

  float& x;
  float& y;
  float& z;
  float& width;
  float& height;
  float& depth;

};

