#include <Windows.h>
#include "cl_Game.h"
#include "Scene_Game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int) {
  Game game("Pongtroids", 800, 600);
  game.initialize<Scene_Game>();
  game.run();

}

