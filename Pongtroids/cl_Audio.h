#pragma once
#include "FMOD/fmod.hpp"
#include <string>
#include "ns_Utility.h"

class Sound;

class Audio {
public:
  Audio();
  ~Audio();

  void update();

  Sound genSound(const std::string& filename);

private:
  FMOD::System* system;

};
