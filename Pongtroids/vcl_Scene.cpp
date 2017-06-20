#include "vcl_Scene.h"
#include "cl_Game.h"

Scene::Scene(SharedState& shared) : shared(shared) {
  //nothing
}

Scene* Scene::update() {
  passiveUpdate();

  if(subScene) {
    Scene* next = subScene->update();
    if(next != subScene.get()) { subScene.reset(next); }

    return this;
  }

  return activeUpdate();

}

void Scene::draw() {
  passiveDraw();
  if(subScene) { subScene->draw(); }
  else         { activeDraw(); }
}
