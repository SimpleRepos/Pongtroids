#include "ns_Utility.h"
#include <fstream>

Utility::OnScopeExit::OnScopeExit(std::function<void()> func) {
  reset(func);
}

Utility::OnScopeExit::~OnScopeExit() {
  func();
}

void Utility::OnScopeExit::reset(std::function<void()> func) {
  this->func = func;
}

void Utility::OnScopeExit::release() {
  reset([]() {});
}

std::vector<char> Utility::readFile(const std::string& filename) {
  std::ifstream file(filename, std::ifstream::binary);
  assert(file);

  file.seekg(0, std::ifstream::end);
  size_t len = (size_t)file.tellg();
  file.seekg(0, std::ifstream::beg);

  std::vector<char> data(len);
  file.read(data.data(), data.size());

  return data;
}

DirectX::XMFLOAT2 Utility::randDirVec(std::mt19937& rng) {
  static std::uniform_real_distribution<float> radianDist(0, PI * 2);
  float radians = radianDist(rng);
  return { std::cosf(radians), std::sinf(radians) };
}

Utility::RandomPositionGenerator::RandomPositionGenerator(float left, float top, float right, float bottom) :
  xDist(left, right),
  yDist(top, bottom)
{
  //nop
}

DirectX::XMFLOAT2 Utility::RandomPositionGenerator::randPos(std::mt19937& rng) {
  return { xDist(rng), yDist(rng) };
}

float Utility::clamp(float val, float min, float max) {
  assert(min <= max);
  if(val < min) { val = min; }
  if(val > max) { val = max; }
  return val;
}
