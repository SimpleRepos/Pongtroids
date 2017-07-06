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

  void setBGMTrack(const std::string& fname);
  void pauseToggleBGM();
  float bgmVolume() const;
  void bgmVolume(float vol);

private:
  void updateVol();

  FMOD::System* system;
  FMOD::Sound* bgm;
  FMOD::Channel* bgmChan;
  float bgmVol;

};

