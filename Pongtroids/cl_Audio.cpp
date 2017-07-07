#include "cl_Audio.h"
#include "cl_Sound.h"

#ifndef _DEBUG
#pragma comment(lib, "FMOD/fmod_vc.lib")
#else
#pragma comment(lib, "FMOD/fmodL_vc.lib")
#endif // !_DEBUG


Audio::Audio() : bgmChan(nullptr), bgmVol(1.0f) {
  FMOD::System* sys;
  FMOD_RESULT sult = FMOD::System_Create(&sys);
  assert(sult == FMOD_OK);
  system.reset(sys);

  sult = system->init(32, FMOD_INIT_NORMAL, 0);
  assert(sult == FMOD_OK);
}

void Audio::update() {
  system->update();
}

Sound Audio::genSound(const std::string& filename) {
  FMOD::Sound* sound;
  FMOD_RESULT sult = system->createSound(filename.c_str(), FMOD_DEFAULT, 0, &sound);
  assert(sult == FMOD_OK);

  return Sound(sound, system.get());
}

void Audio::setBGMTrack(const std::string& fname) {
  FMOD::Sound* tmp;
  FMOD_RESULT sult = system->createStream(fname.c_str(), FMOD_LOOP_NORMAL, 0, &tmp);
  assert(sult == FMOD_OK);
  bgm.reset(tmp);

  sult = system->playSound(bgm.get(), 0, true, &bgmChan);
  assert(sult == FMOD_OK);

  updateVol();
  pauseToggleBGM();
}

void Audio::pauseToggleBGM() {
  if(!bgm) { return; }

  bool wtf;
  FMOD_RESULT sult = bgmChan->getPaused(&wtf);
  assert(sult == FMOD_OK);
  sult = bgmChan->setPaused(!wtf);
  assert(sult == FMOD_OK);
}

float Audio::bgmVolume() const {
  return bgmVol;
}

void Audio::bgmVolume(float vol) {
  bgmVol = vol;
  updateVol();
}

void Audio::updateVol() {
  if(!bgm) { return; }

  FMOD_RESULT sult = bgmChan->setVolume(bgmVol);
  assert(sult == FMOD_OK);
}

