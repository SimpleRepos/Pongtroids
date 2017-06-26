#include "cl_Paddles.h"
#include "cl_Camera.h"

namespace {
  std::vector<Vertex::Pos3Norm3Tex2> squareVerts = {
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, -1), DirectX::XMFLOAT2(0, 0) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(1, 0, 0), DirectX::XMFLOAT3(0, 0, -1), DirectX::XMFLOAT2(1, 0) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(0, 1, 0), DirectX::XMFLOAT3(0, 0, -1), DirectX::XMFLOAT2(0, 1) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(0, 1, 0), DirectX::XMFLOAT3(0, 0, -1), DirectX::XMFLOAT2(0, 1) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(1, 0, 0), DirectX::XMFLOAT3(0, 0, -1), DirectX::XMFLOAT2(1, 0) },
    Vertex::Pos3Norm3Tex2{ DirectX::XMFLOAT3(1, 1, 0), DirectX::XMFLOAT3(0, 0, -1), DirectX::XMFLOAT2(1, 1) },
  };
}

Paddles::Paddles(SharedState& shared, RenderProgram<DirectX::XMFLOAT4X4>& spriteProg) :
  TOP_BOUND(0),
  BOT_BOUND(shared.gfx.VIEWPORT_DIMS.height - HEIGHT),
  shared(shared),
  mesh(shared.factory.createStaticMeshFromVertices(squareVerts)),
  tex(shared.factory.createTexture(L"../Assets/paddleBlu.png")),
  prog(&spriteProg)
{
  auto& vp = shared.gfx.VIEWPORT_DIMS;

  xforms[LEFT].translation = { HORZ_BORDER_GAP, (vp.height - HEIGHT) / 2, 0 };
  xforms[LEFT].scale = { WIDTH, HEIGHT, 1 };

  xforms[RIGHT] = xforms[LEFT];
  xforms[RIGHT].x = vp.width - (WIDTH + HORZ_BORDER_GAP);

  colliders[LEFT].x(xforms[LEFT].x);
  colliders[LEFT].y(xforms[LEFT].y);
  colliders[LEFT].width(xforms[LEFT].width);
  colliders[LEFT].height(xforms[LEFT].height);

  colliders[RIGHT] = colliders[LEFT];
  colliders[RIGHT].x(xforms[RIGHT].x);
}

void Paddles::update(float dt) {
  float y = xforms[LEFT].y;
  if(shared.input.keyboard().buttons[VK_UP].held)   { y -= SPEED * dt; }
  if(shared.input.keyboard().buttons[VK_DOWN].held) { y += SPEED * dt; }
  y = Utility::clamp(y, TOP_BOUND, BOT_BOUND);

  xforms[LEFT].y = y;
  colliders[LEFT].y(y);
  xforms[RIGHT].y = y;
  colliders[RIGHT].y(y);
}

void Paddles::draw(Camera& cam) {
  tex.set(0);

  prog->set();

  prog->cBuffer.object = cam.getTransposedWVP(xforms[LEFT]);
  prog->cBuffer.update();
  shared.gfx.draw(4);

  prog->cBuffer.object = cam.getTransposedWVP(xforms[RIGHT]);
  prog->cBuffer.update();
  shared.gfx.draw(4);
}

DirectX::XMFLOAT2 Paddles::getDeflectionDirection(Side side, float ballY) const {
  auto& paddle = colliders[side];

  //calculate normalized offset such that the ball touching the top of the paddle is -1 and the bottom is +1
  float halfHeight = paddle.height() / 2;
  float center = paddle.top + halfHeight;
  float normalizedOffset = (ballY - center) / halfHeight;

  float theta = MAX_DEFLECTION_ANGLE * normalizedOffset;

  DirectX::XMFLOAT2 dir{ cosf(theta), sinf(theta) };
  if(side == Paddles::RIGHT) { dir.x = -dir.x; }

  return dir;
}
