#include "cl_ScoreBoard.h"
#include "ns_Utility.h"

ScoreBoard::ScoreBoard(SharedState& shared) : 
  shared(shared),
  scoreFont(shared.factory.createFont(L"Courier")),
  prev{ -1, -1 }
{
  //nop
}

void ScoreBoard::update() {
  if(prev[LIVES] != shared.gameState.lives) {
    stringTable[LIVES] = Utility::stringf(L"Lives: %d", shared.gameState.lives);
    prev[LIVES] = shared.gameState.lives;
  }
  if(prev[SCORE] != shared.gameState.score) {
    stringTable[SCORE] = Utility::stringf(L"Score: %d", shared.gameState.score);
    prev[SCORE] = shared.gameState.score;
  }

}

void ScoreBoard::draw() {
  scoreFont.drawText(stringTable[LIVES], 24, 5, 5, ColorF::YELLOW);
  scoreFont.drawText(stringTable[SCORE], 24, 600, 5, ColorF::YELLOW);

}
