#include "cl_Transform.h"

using namespace DirectX;

void Transform::translate(DirectX::XMFLOAT3 offset) {
  translation.x += offset.x;
  translation.y += offset.y;
  translation.z += offset.z;
}

void Transform::rotate(DirectX::XMFLOAT3 axis, float radians) {
  XMStoreFloat4(
    &rotationQuaternion, 
    XMQuaternionMultiply(
      XMLoadFloat4(&rotationQuaternion), 
      XMQuaternionRotationAxis(XMLoadFloat3(&axis), radians)
    )
  );
}

void Transform::mulScale(DirectX::XMFLOAT3 factors) {
  scale.x *= factors.x;
  scale.y *= factors.y;
  scale.z *= factors.z;
}

Transform::operator DirectX::XMFLOAT4X4() const {
  XMVECTOR vOrigin   = XMVectorZero();
  XMVECTOR vRot      = XMLoadFloat4(&rotationQuaternion);
  XMVECTOR vTrans    = XMLoadFloat3(&translation);
  XMVECTOR vScale    = XMLoadFloat3(&scale);
  XMVECTOR vScaleRot = XMVectorZero();

  XMFLOAT4X4 worldMat;
  XMStoreFloat4x4(&worldMat, XMMatrixTransformation(vOrigin, vScaleRot, vScale, vOrigin, vRot, vTrans));
  return worldMat;
}
