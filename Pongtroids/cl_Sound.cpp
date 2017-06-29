#include "cl_Sound.h"

Sound::~Sound() {
  sound->release();
}

void Sound::play() {
  FMOD::Channel* chan;
  FMOD_RESULT sult = sys->playSound(sound, nullptr, true, &chan);
  assert(sult == FMOD_OK);
  sult = chan->setVolume(vol);
  assert(sult == FMOD_OK);
  sult = chan->setPaused(false);
  assert(sult == FMOD_OK);
}

void Sound::volume(float vol) {
  this->vol = vol;
}

Sound::Sound(const std::string& fname, FMOD::System* sys) : sys(sys), vol(1) {
  FMOD_RESULT sult = sys->createSound(fname.c_str(), FMOD_DEFAULT, 0, &sound);
  assert(sult == FMOD_OK);
}

