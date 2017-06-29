#pragma once
#include "FMOD\fmod.hpp"
#include "ns_Utility.h"
#include <string>

class Sound {
public:
  ~Sound();

  void play();
  void volume(float vol);

private:
  friend class Audio;

  Sound(const std::string& fname, FMOD::System* sys);

  FMOD::Sound* sound;
  FMOD::System* sys;

  float vol;

};

