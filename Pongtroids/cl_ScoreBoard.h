#pragma once
#include "cl_Font.h"
#include "st_SharedState.h"
#include <vector>
#include <string>

class ScoreBoard {
public:
  ScoreBoard(SharedState& shared);
  void update();
  void draw();

  void pushFlair(float x, float y);

private:
  SharedState& shared;
  Font scoreFont;

  enum TableIndex {
    LIVES,
    SCORE
  };

  std::array<int, 2> prev;
  std::array<std::wstring, 2> stringTable;

  struct Flair {
    Flair(float x, float y, int mul);
    void draw(Font& font);

    static constexpr float BASE_SIZE = 24;
    static constexpr int SIZE_RATE = 4;
    static constexpr float DURATION = 1.75f;
    static constexpr float BLINK_RATE = 0.125f;

    std::wstring text;
    float size;
    float timeRemaining;
    float x, y;
  };

  std::vector<Flair> multiplierFlairs;

};
