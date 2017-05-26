#pragma once
#include "cl_Window.h"
#include "cl_Graphics.h"
#include "cl_Timer.h"
#include "cl_GfxFactory.h"
#include "cl_Input.h"
#include <random>

///<summary>Storage of centralized objects and access point for scenes to share persistent state</summary>
struct SharedState {
  SharedState(const std::string& windowTitle, Window::Dimensions dims) : 
    win(windowTitle, dims),
    gfx(win),
    factory(gfx.createFactory()),
    input(win),
    rng(std::random_device{}())
  {
    //nothing
  }

  Window win;
  Graphics gfx;
  GfxFactory factory;
  Timer timer;
  Input input;
  std::mt19937 rng;

};
