#include "vcl_Scene.h"
#include "cl_Game.h"

Scene::Scene(SharedState& shared) : shared(shared) {
  //nothing
}

Scene* Scene::update() {
  passiveUpdate();
  if(subState) { return subState->update(); }
  else         { return activeUpdate(); }
}

void Scene::draw() {
  passiveDraw();
  if(subState) { subState->draw(); }
  else         { activeDraw(); }
}
