#include "Scene_Game.h"
#include "st_SharedState.h"
#include "cl_GfxFactory.h"
#include <random>
#include <sstream>

using namespace DirectX;

Scene_Game::Scene_Game(SharedState& shared) : Scene(shared) {
  shared.win.addKeyFunc(VK_ESCAPE, [](HWND, LPARAM) { PostQuitMessage(0); });

}

Scene* Scene_Game::update() {

  return this;
}

void Scene_Game::draw() {
  shared.gfx.clear(ColorF::CYAN);

}

