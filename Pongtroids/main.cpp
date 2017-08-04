#include <Windows.h>
#include "cl_Game.h"
#include "Scene_Title.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int) {
  Game game("Pongtroids", 800, 600);
  game.initialize<Scene_Title>();
  game.run();

}

