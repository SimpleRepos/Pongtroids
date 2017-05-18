#pragma once
#include <DirectXMath.h>
#include "cl_StaticVertexBuffer.h"
#include "cl_Texture.h"

class StaticMeshSprite {
public:
  StaticMeshSprite(const StaticVertexBuffer* vBuffer, const Texture* texture);

  DirectX::XMFLOAT3 getTranslation() const;
  DirectX::XMFLOAT4 getRotationQuaternion() const;
  DirectX::XMFLOAT3 getScale() const;
  DirectX::XMFLOAT4X4 getMatrix() const;

  void setTranslation(DirectX::XMFLOAT3 translation);
  void setRotationQuaternion(DirectX::XMFLOAT4 rotation);
  void setScale(DirectX::XMFLOAT3 scale);

  void translate(DirectX::XMFLOAT3 translation);
  void rotate(DirectX::XMFLOAT3 axis, float radians);
  void mulScale(DirectX::XMFLOAT3 scale);

  void draw();

private:
  void updateMatrix() const;

  const StaticVertexBuffer* m_vBuffer;
  const Texture* m_texture;
  DirectX::XMFLOAT3 m_translation;
  DirectX::XMFLOAT4 m_rotQuat;
  DirectX::XMFLOAT3 m_scale;
  mutable DirectX::XMFLOAT4X4 m_worldMat;
  mutable bool m_dirty;

};

/////////////////////////////////////////////

DirectX::XMFLOAT3& operator+=(DirectX::XMFLOAT3& target, DirectX::XMFLOAT3 addend) { //~~@ this should be elsewhere
  target.x += addend.x;
  target.y += addend.y;
  target.z += addend.z;
  return target;
}

/////////////////////////////////////////////

using namespace DirectX;

StaticMeshSprite::StaticMeshSprite(const StaticVertexBuffer* vBuffer, const Texture* texture) :
  m_vBuffer(vBuffer),
  m_texture(texture),
  m_translation(0,0,0),
  m_rotQuat(0,0,0,1),
  m_scale(1,1,1),
  m_worldMat{0},
  m_dirty(true)
{
  //nop
}

DirectX::XMFLOAT4X4 StaticMeshSprite::getMatrix() const {
  if(m_dirty) { updateMatrix(); }
  return m_worldMat;
}

DirectX::XMFLOAT3 StaticMeshSprite::getTranslation() const {
  return m_translation;
}

DirectX::XMFLOAT4 StaticMeshSprite::getRotationQuaternion() const {
  return m_rotQuat;
}

DirectX::XMFLOAT3 StaticMeshSprite::getScale() const {
  return m_scale;
}

void StaticMeshSprite::setTranslation(DirectX::XMFLOAT3 translation) {
  m_dirty = true;
  m_translation = translation;
}

void StaticMeshSprite::setRotationQuaternion(DirectX::XMFLOAT4 quaternion) {
  m_dirty = true;
  m_rotQuat = quaternion;
}

void StaticMeshSprite::setScale(DirectX::XMFLOAT3 scale) {
  m_dirty = true;
  m_scale = scale;
}

void StaticMeshSprite::translate(DirectX::XMFLOAT3 translation) {
  m_dirty = true;
  m_translation += translation;
}

void StaticMeshSprite::rotate(DirectX::XMFLOAT3 axis, float radians) {
  m_dirty = true;
  XMStoreFloat4(
    &m_rotQuat,
    XMQuaternionMultiply( //~~! this multiplication may be in the wrong order. test this function carefully with compound rotations
      XMLoadFloat4(&m_rotQuat),
      XMQuaternionRotationAxis(XMLoadFloat3(&axis), radians)
    )
  );

}

void StaticMeshSprite::mulScale(DirectX::XMFLOAT3 scale) {
  m_dirty = true;
  m_scale.x *= scale.x;
  m_scale.y *= scale.y;
  m_scale.z *= scale.z;
}

void StaticMeshSprite::draw() {
  //~~_
}

void StaticMeshSprite::updateMatrix() const {
  XMStoreFloat4x4(
    &m_worldMat, 
    XMMatrixTransformation(
      XMVectorZero(), XMQuaternionIdentity(), XMLoadFloat3(&m_scale),
      XMVectorZero(), XMLoadFloat4(&m_rotQuat),
      XMLoadFloat3(&m_translation)
    )
  );

  m_dirty = false;
}


