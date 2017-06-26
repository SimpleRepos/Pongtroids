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

private:
  SharedState& shared;
  Font scoreFont;

  enum TableIndex {
    LIVES,
    SCORE
  };

  std::array<int, 2> prev;
  std::array<std::wstring, 2> stringTable;

};
