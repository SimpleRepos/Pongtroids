#pragma once
#include "FMOD/fmod.hpp"
#include <string>
#include "ns_Utility.h"
#include <memory>

class Sound;

namespace std {
  template<>
  struct default_delete<FMOD::Sound> {
    void operator()(FMOD::Sound* p) { p->release(); }
  };

  template<>
  struct default_delete<FMOD::System> {
    void operator()(FMOD::System* p) { p->release(); }
  };
}

class Audio {
public:
  Audio();

  void update();

  Sound genSound(const std::string& filename);

  void setBGMTrack(const std::string& fname);
  void pauseToggleBGM();
  float bgmVolume() const;
  void bgmVolume(float vol);

private:
  void updateVol();

  std::unique_ptr<FMOD::System> system;
  std::unique_ptr<FMOD::Sound> bgm;
  FMOD::Channel* bgmChan;
  float bgmVol;

};

