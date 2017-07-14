#pragma once

struct GameState {
  void scoreUp(int preMulAmt) {
    score += preMulAmt * multiplier;
    multiplier++;
  }

  void resetMultiplier() {
    multiplier = 1;
  }

  int lives = 3;
  int score = 0;
  int multiplier = 1;
};
