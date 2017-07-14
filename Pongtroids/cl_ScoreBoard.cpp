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

  for(auto& flair : multiplierFlairs) { flair.timeRemaining -= (float)shared.timer.getTickDT(); }
  auto expired = [](const Flair& f) { return f.timeRemaining <= 0; };
  multiplierFlairs.erase(std::remove_if(multiplierFlairs.begin(), multiplierFlairs.end(), expired), multiplierFlairs.end());

}

void ScoreBoard::draw() {
  scoreFont.drawText(stringTable[LIVES], 24, 5, 5, ColorF::YELLOW);
  scoreFont.drawText(stringTable[SCORE], 24, 670, 5, ColorF::YELLOW);
  for(auto& f : multiplierFlairs) { f.draw(scoreFont); }
}

void ScoreBoard::pushFlair(float x, float y) {
  if(shared.gameState.multiplier > 1) {
    multiplierFlairs.emplace_back(x, y, shared.gameState.multiplier);
  }
}

ScoreBoard::Flair::Flair(float x, float y, int mul) : 
  text(Utility::stringf(L"x%d", mul)),
  size(BASE_SIZE + (mul * SIZE_RATE)),
  timeRemaining(DURATION),
  x(x), y(y)
{
  //nop
}

void ScoreBoard::Flair::draw(Font& font) {
  int colSw = (int)(timeRemaining / BLINK_RATE) & 1;

  auto color = colSw ? ColorF::RED : ColorF::YELLOW;
  if(timeRemaining < 1) { color.a = timeRemaining; }

  font.drawText(text, size, x, y, color);
}
