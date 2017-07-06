#include "cl_Audio.h"
#include "cl_Sound.h"

#ifndef _DEBUG
#pragma comment(lib, "FMOD/fmod_vc.lib")
#else
#pragma comment(lib, "FMOD/fmodL_vc.lib")
#endif // !_DEBUG


Audio::Audio() : system(nullptr), bgm(nullptr), bgmChan(nullptr), bgmVol(1.0f) {
  FMOD_RESULT sult = FMOD::System_Create(&system);
  assert(sult == FMOD_OK);

  //exception may happen soon and dtor won't fire, so ensure that system pointer is raii in this funciton
  Utility::OnScopeExit raii([this]() { system->release(); });
  
  sult = system->init(512, FMOD_INIT_NORMAL, 0);
  assert(sult == FMOD_OK);

  //validate the system pointer (won't be released in this function)
  raii.release();
}

Audio::~Audio() {
  if(bgm) { bgm->release(); }
  system->release();
}

void Audio::update() {
  system->update();
}

Sound Audio::genSound(const std::string& filename) {
  return Sound(filename, system);
}

void Audio::setBGMTrack(const std::string& fname) {
  FMOD_RESULT sult = system->createStream(fname.c_str(), FMOD_LOOP_NORMAL, 0, &bgm);
  assert(sult == FMOD_OK);
  sult = system->playSound(bgm, 0, true, &bgmChan);
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

