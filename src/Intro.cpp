#include "Intro.h"
#include "utility/tools.h"

#include <loadpng.h>
#include <string>
#include <vector>
#include <string>

#include "globals.h"
#include "utility/tools.h"

Intro::Intro() {
  background = load_png_ex("images/opening/background.png");
  intro = load_png_ex("images/opening/intro.png");
  title = load_png_ex("images/opening/title.png");
  introSound = load_sample_ex("sounds/introSound.wav");

  for (int i = 0; i < INTRO_FRAMES; i++) {
    images[i] = load_png_ex(std::string("images/opening/opening" + std::to_string(i) + ".png").c_str());
  }

  timer.Start();
  highcolor_fade_in(intro, 32);
  frame = 0;
  sound_played = false;
}

Intro::~Intro() {
  for (int i = 0; i < INTRO_FRAMES; i++)
    destroy_bitmap (images[i]);

  destroy_bitmap (background);
  destroy_bitmap (title);
  destroy_bitmap (intro);
  destroy_sample (introSound);
}


void Intro::update(StateEngine *engine) {
  frame = (timer.GetElapsedTime<milliseconds>() - 3000) / 100;
  if (frame >= 0 && !sound_played) {
    play_sample(introSound, 255, 128, 1000, 0);
    sound_played = true;
  }

  if (frame >= INTRO_FRAMES || key_down() || button_down()) {
    setNextState (engine, StateEngine::STATE_MENU);
    highcolor_fade_out(64);
  }
}

void Intro::draw(BITMAP *buffer) {
  // Intro stuffs
  if (timer.GetElapsedTime<seconds>() < 1) {
    draw_sprite(buffer, intro, 0, 0);
  }
  else if (timer.GetElapsedTime<seconds>() < 2) {
    draw_sprite(buffer, title, 0, 0);
  }
  else {
    clear_to_color(buffer, 0x00000);
    stretch_sprite(buffer, background, 105, 140, 1070, 680);

    if (frame >= 0 && frame < INTRO_FRAMES)
      stretch_sprite(buffer, images[frame], 105, 120, 1070, 660);
  }
}
