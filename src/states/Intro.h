#pragma once

#include <asw/asw.h>
#include "../Timer.h"

#include "./State.h"

constexpr int INTRO_FRAMES = 84;

// Intro screen of game
class Intro : public asw::scene::Scene<ProgramState> {
 public:
  using asw::scene::Scene<ProgramState>::Scene;

  void init() override;
  void update(float dt) override;
  void draw() override;
  void cleanup() override;

 private:
  asw::Texture intro;
  asw::Texture title;
  asw::Texture background;

  // Only the frame on screen is loaded, frames are streamed from disk
  asw::Texture current_frame;
  int loaded_frame = -1;
  asw::Sample introSound;

  int frame = 0;
  bool sound_played = false;

  Timer timer;
};
