#include "cl_Audio.h"
#include "cl_Sound.h"

#ifndef _DEBUG
#pragma comment(lib, "FMOD/fmod_vc.lib")
#else
#pragma comment(lib, "FMOD/fmodL_vc.lib")
#endif // !_DEBUG


Audio::Audio() {
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
  system->release();
}

void Audio::update() {
  system->update();
}

Sound Audio::genSound(const std::string& filename) {
  return Sound(filename, system);
}
