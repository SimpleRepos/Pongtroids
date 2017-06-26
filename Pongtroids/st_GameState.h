#pragma once

struct GameState {
  bool operator==(const GameState& other) {
    return (lives == other.lives) && (score == other.score);
  }

  bool operator!=(const GameState& other) {
    return !operator==(other);
  }

  int lives = 3;
  int score = 0;
};
