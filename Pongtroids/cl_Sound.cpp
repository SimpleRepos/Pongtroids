#include "cl_Sound.h"

Sound::~Sound() {
  sound->release();
}

void Sound::play() {
  FMOD_RESULT sult = sys->playSound(sound, 0, false, nullptr);
  assert(sult == FMOD_OK);
}

Sound::Sound(const std::string& fname, FMOD::System* sys) : sys(sys) {
  FMOD_RESULT sult = sys->createSound(fname.c_str(), FMOD_DEFAULT, 0, &sound);
  assert(sult == FMOD_OK);
}
