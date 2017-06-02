#pragma once
#include <functional>
#include <vector>
#include <cassert>
#include <random>
#include <array>
#include <DirectXMath.h>

#define HR(a) { HRESULT hr = a; assert(SUCCEEDED(hr)); }

///<summary>Utility functions and classes</summary>
namespace Utility {
  constexpr float PI = 3.141592654f;

  ///<summary>Executes a function on destruction</summary>
  ///<remarks>This class can be used to easily implement exception-safe design patterns without explicitly coding unusual RAII classes</remarks>
  class OnScopeExit {
  public:
    ///<summary>Generate object that will call its associated function when it leaves scope</summary>
    ///<param name="func">The function to be executed when this object destructs</param>
    OnScopeExit(std::function<void()> func);

    ~OnScopeExit();

    ///<summary>Set a new associated function, replaceing the previous one</summary>
    ///<param name="func">The function to be executed when this object destructs</param>
    void reset(std::function<void()> func);

    ///<summary>Set the associated function to an empty one</summary>
    void release();

  private:
    std::function<void()> func;

  };

  ///<summary>Read a file into a vector of char (binary read)</summary>
  ///<param name="filename">Path to the file to be read</param>
  std::vector<char> readFile(const std::string& filename);

  ///<summary>Generate a unit-length float vector of 2 dimensions</summary>
  DirectX::XMFLOAT2 randDirVec(std::mt19937& rng);

  ///<summary>Specify a set of bounds and generate random 2D positions within it</summary>
  struct RandomPositionGenerator {
    RandomPositionGenerator(float left, float top, float right, float bottom);
    DirectX::XMFLOAT2 randPos(std::mt19937& rng);

    std::uniform_real_distribution<float> xDist, yDist;
  };

  ///<summary>Clamp a value to a range</summary>
  float clamp(float val, float min, float max);

}

