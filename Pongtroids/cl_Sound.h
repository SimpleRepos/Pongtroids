#pragma once
#include "FMOD\fmod.hpp"
#include "ns_Utility.h"
#include <string>
#include "cl_Audio.h"

class Sound {
public:
  void play();
  void volume(float vol);

private:
  friend class Audio;
  Sound(FMOD::Sound* sound, FMOD::System* sys);

  std::unique_ptr<FMOD::Sound> sound;
  FMOD::Channel* chan;
  FMOD::System* sys;
  float vol;

};

