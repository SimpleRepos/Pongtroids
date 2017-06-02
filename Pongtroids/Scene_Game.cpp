#include "Scene_Game.h"
#include "st_SharedState.h"
#include "cl_GfxFactory.h"
#include <vector>
#include <cmath>
#include <fstream>
#include "ns_Vertex.h"
#include "cl_Window.h"
#include "ns_Utility.h"


Scene_Game::Scene_Game(SharedState& shared) : 
  Scene(shared),
  regions(shared),
  renderProg(shared),
  entities(shared, regions, 3),
  bg(shared, renderProg, regions)
{
  shared.win.addKeyFunc(VK_ESCAPE, [](HWND, LPARAM) { PostQuitMessage(0); });
  renderProg.set();
}

Scene* Scene_Game::activeUpdate() {
  entities.update((float)shared.timer.getTickDT(), regions);
  return this;
}

void Scene_Game::activeDraw() {
  shared.gfx.clear(ColorF::CYAN);
  bg.draw(renderProg);
  entities.draw(renderProg);
}

