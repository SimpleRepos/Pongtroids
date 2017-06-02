#include "cl_Transform.h"

using namespace DirectX;

Transform::Transform(XMFLOAT3 translation, XMFLOAT3 scale, XMFLOAT4 rotationQuaternion) :
  translation(translation),
  scale(scale),
  rotationQuaternion(rotationQuaternion),
  x(this->translation.x),
  y(this->translation.y),
  z(this->translation.z),
  width(this->scale.x),
  height(this->scale.y),
  depth(this->scale.z)
{
  //nop
}

Transform::Transform(const Transform& other) :
  translation(other.translation),
  scale(other.scale),
  rotationQuaternion(other.rotationQuaternion),
  x(translation.x),
  y(translation.y),
  z(translation.z),
  width(scale.x),
  height(scale.y),
  depth(scale.z)
{
  //nop
}

Transform& Transform::operator=(const Transform& other) {
  translation = other.translation;
  scale = other.scale;
  rotationQuaternion = other.rotationQuaternion;
  return *this;
}

void Transform::translate(DirectX::XMFLOAT3 offset) {
  translation.x += offset.x;
  translation.y += offset.y;
  translation.z += offset.z;
}

void Transform::translate(DirectX::XMVECTOR& offset) {
  XMStoreFloat3(
    &translation,
    XMLoadFloat3(&translation) + offset
  );
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

void Transform::rotate(DirectX::XMVECTOR& axis, float radians) {
  XMStoreFloat4(
    &rotationQuaternion, 
    XMQuaternionMultiply(
      XMLoadFloat4(&rotationQuaternion), 
      XMQuaternionRotationAxis(axis, radians)
    )
  );
}

void Transform::mulScale(DirectX::XMFLOAT3 factors) {
  scale.x *= factors.x;
  scale.y *= factors.y;
  scale.z *= factors.z;
}

void Transform::mulScale(float uniformFactor) {
  mulScale({ uniformFactor, uniformFactor, uniformFactor });
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
