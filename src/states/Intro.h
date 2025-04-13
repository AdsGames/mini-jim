#pragma once

#include <asw/asw.h>
#include <asw/util/Timer.h>

#include "./State.h"

constexpr int INTRO_FRAMES = 84;

// Intro screen of game
class Intro : public asw::scene::Scene<ProgramState> {
 public:
  using asw::scene::Scene<ProgramState>::Scene;

  void init() override;
  void update(float dt) override;
  void draw() override;

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
