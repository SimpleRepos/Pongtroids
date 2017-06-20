#pragma once
#include <memory>

struct SharedState;

///<summary>Abstract base class for scenes - allows polymorphism in the top-level state machine</summary>
class Scene {
public:
  Scene(SharedState& shared);
  virtual ~Scene() = default;

  ///<summary>Update the simulation by SharedState::Timer::getTickDT() seconds</summary>
  Scene* update();
  virtual void passiveUpdate() {}
  virtual Scene* activeUpdate() = 0;

  ///<summary>Render the scene</summary>
  void draw();
  virtual void passiveDraw() {}
  virtual void activeDraw() = 0;

protected:
  SharedState& shared;
  std::unique_ptr<Scene> subScene;

};
