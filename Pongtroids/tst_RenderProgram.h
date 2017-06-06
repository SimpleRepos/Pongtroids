#pragma once
#include "st_ShaderSet.h"
#include "tcl_ConstantBuffer.h"

template<class T>
struct RenderProgram {
  ShaderSet shaders;
  ConstantBuffer<T> cBuffer;

  void set() {
    shaders.set();
    cBuffer.set(0);
  }
};

