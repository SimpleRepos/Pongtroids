#include "cl_Transform.h"

using namespace DirectX;

void Transform::translate(DirectX::XMFLOAT3 offset) {
  translation.x += offset.x;
  translation.y += offset.y;
  translation.z += offset.z;
}

void Transform::rotate(DirectX::XMFLOAT3 axis, float radians) {
  auto vCurRot = XMLoadFloat4(&rotationQuaternion);
  auto vAddRot = XMQuaternionRotationAxis(XMLoadFloat3(&axis), radians);

  //~~! this multiplication may be in the wrong order. test this function carefully with compound rotations
  auto vNewRot = XMQuaternionMultiply(vCurRot, vAddRot);

  XMStoreFloat4(&rotationQuaternion, vNewRot);
}

void Transform::mulScale(DirectX::XMFLOAT3 factors) {
  scale.x *= factors.x;
  scale.y *= factors.y;
  scale.z *= factors.z;
}

DirectX::XMFLOAT4X4 Transform::asMatrix() const {
  XMVECTOR vOrigin = XMVectorZero();
  XMVECTOR vRot    = XMLoadFloat4(&rotationQuaternion);
  XMVECTOR vTrans  = XMLoadFloat3(&translation);
  XMVECTOR vScale  = XMLoadFloat3(&scale);
  XMVECTOR& vScaleRot = vRot; //~~! this needs tested

  XMFLOAT4X4 worldMat;
  XMStoreFloat4x4(&worldMat, XMMatrixTransformation(vOrigin, vScaleRot, vScale, vOrigin, vRot, vTrans));
  return worldMat;
}


