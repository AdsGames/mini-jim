#ifndef INTRO_H
#define INTRO_H

#include <asw/asw.h>
#include <asw/util/Timer.h>

#include "State.h"

#define INTRO_FRAMES 84

// Intro screen of game
class Intro : public State {
 public:
  explicit Intro(StateEngine& engine) : State(engine) {}

  virtual void init() override;
  virtual void update() override;
  virtual void draw() override;
  virtual void cleanup() override{};

 private:
  asw::Texture intro;
  asw::Texture title;
  asw::Texture background;
  asw::Texture images[INTRO_FRAMES];
  asw::Sample introSound;

  int frame = 0;
  bool sound_played = false;

  Timer timer;
};

#endif  // INTRO_H
