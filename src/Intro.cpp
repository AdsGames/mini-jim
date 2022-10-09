#include "Intro.h"
#include "utility/tools.h"

#include <asw/util/KeyListener.h>
#include <string>
#include <vector>

#include "globals.h"
#include "utility/tools.h"

void Intro::init() {
  background = asw::load::texture("assets/images/opening/background.png");
  intro = asw::load::texture("assets/images/opening/intro.png");
  title = asw::load::texture("assets/images/opening/title.png");
  introSound = asw::load::sample("assets/sounds/introSound.wav");

  for (int i = 0; i < INTRO_FRAMES; i++) {
    images[i] = asw::load::texture("assets/images/opening/opening" +
                                   std::to_string(i) + ".png");
  }

  timer.Start();
  frame = 0;
  sound_played = false;
}

void Intro::update() {
  // poll_joystick();
  frame = (timer.GetElapsedTime<milliseconds>() - 3000) / 100;

  if (frame >= 0 && !sound_played) {
    asw::sound::play(introSound, 255, 128, 0);
    sound_played = true;
  }

  if (frame >= INTRO_FRAMES || KeyListener::anyKeyPressed) {
    setNextState(StateEngine::STATE_MENU);
  }
}

void Intro::draw() {
  // Intro stuffs
  if (timer.GetElapsedTime<seconds>() < 1) {
    asw::draw::sprite(intro, 0, 0);
  } else if (timer.GetElapsedTime<seconds>() < 2) {
    asw::draw::sprite(title, 0, 0);
  } else {
    asw::draw::clearColor(asw::util::makeColor(0, 0, 0));
    asw::draw::stretchSprite(background, 105, 140, 1070, 680);

    if (frame >= 0 && frame < INTRO_FRAMES) {
      asw::draw::stretchSprite(images[frame], 105, 120, 1070, 660);
    }
  }
}
