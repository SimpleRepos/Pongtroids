#include "cl_Sound.h"

void Sound::play() {
  FMOD_RESULT sult = sys->playSound(sound.get(), 0, true, &chan);
  assert(sult == FMOD_OK);
  sult = chan->setVolume(vol);
  assert(sult == FMOD_OK);
  sult = chan->setPaused(false);
  assert(sult == FMOD_OK);
}

void Sound::volume(float vol) {
  this->vol = vol;
}

Sound::Sound(FMOD::Sound* sound, FMOD::System* sys) : sound(sound), sys(sys), vol(1) {
  //nop
}

