#pragma once

struct Regions {
  SC::Rect middle;
  SC::Rect left;
  SC::Rect top;
  SC::Rect right;
  SC::Rect bottom;
};

struct ColliderSet {
  const Regions& regions;
  const SC::Rect& lPaddle;
  const SC::Rect& rPaddle;
  std::vector<const Asteroid*> asteroids;
};

