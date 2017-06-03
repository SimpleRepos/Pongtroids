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
  constexpr float MAX_FRAME_LENGTH = 1.0f / 60;
  float dt = Utility::clamp((float)shared.timer.getTickDT(), 0, MAX_FRAME_LENGTH);
  entities.update(dt, regions);
  if(entities.ball.xform.translation.x < regions.left.left)   { MessageBoxA(0, "You lost.", 0, 0); return new Scene_Game(shared); }
  if(entities.ball.xform.translation.x > regions.right.right) { MessageBoxA(0, "You lost.", 0, 0); return new Scene_Game(shared); }
  return this;
}

void Scene_Game::activeDraw() {
  shared.gfx.clear(ColorF::CYAN);
  bg.draw(renderProg);
  entities.draw(renderProg);
}

