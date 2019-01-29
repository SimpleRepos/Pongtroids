#include "ns_Utility.h"
#include <fstream>
#include <cstdarg>

Utility::OnScopeExit::OnScopeExit(std::function<void()> func) {
  resetBall(func);
}

Utility::OnScopeExit::~OnScopeExit() {
  func();
}

void Utility::OnScopeExit::resetBall(std::function<void()> func) {
  this->func = func;
}

void Utility::OnScopeExit::release() {
  resetBall([]() {});
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

std::string Utility::stringf(const char* format, ...) {
  va_list args;
  va_start(args, format);
  std::vector<char> buffer(_vscprintf(format, args) + 1);
  _vsnprintf_s(buffer.data(), buffer.size(), buffer.size(), format, args);
  va_end(args);

  return buffer.data();
}


std::wstring Utility::stringf(const wchar_t* format, ...) {
  va_list args;
  va_start(args, format);
  std::vector<wchar_t> buffer(_vscwprintf(format, args) + 1);
  _vsnwprintf_s(buffer.data(), buffer.size(), buffer.size(), format, args);
  va_end(args);

  return buffer.data();
}


