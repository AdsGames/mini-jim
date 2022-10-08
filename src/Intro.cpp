#include "Intro.h"
#include "utility/tools.h"

#include <string>
#include <vector>

#include "globals.h"
#include "utility/KeyListener.h"
#include "utility/tools.h"

void Intro::init() {
  background = aar::load::bitmap("assets/images/opening/background.png");
  intro = aar::load::bitmap("assets/images/opening/intro.png");
  title = aar::load::bitmap("assets/images/opening/title.png");
  introSound = aar::load::sample("assets/sounds/introSound.wav");

  for (int i = 0; i < INTRO_FRAMES; i++) {
    images[i] = aar::load::bitmap("assets/images/opening/opening" +
                                  std::to_string(i) + ".png");
  }

  timer.Start();
  frame = 0;
  sound_played = false;
}

Intro::~Intro() {
  for (int i = 0; i < INTRO_FRAMES; i++)
    aar::load::destroyTexture(images[i]);

  aar::load::destroyTexture(background);
  aar::load::destroyTexture(title);
  aar::load::destroyTexture(intro);
  aar::load::destroySample(introSound);
}

void Intro::update(StateEngine& engine) {
  // poll_joystick();
  frame = (timer.GetElapsedTime<milliseconds>() - 3000) / 100;

  if (frame >= 0 && !sound_played) {
    aar::sound::play(introSound, 255, 128, 0);
    sound_played = true;
  }

  if (frame >= INTRO_FRAMES || KeyListener::anyKeyPressed) {
    setNextState(engine, StateEngine::STATE_MENU);
  }
}

void Intro::draw() {
  // Intro stuffs
  if (timer.GetElapsedTime<seconds>() < 1) {
    aar::draw::sprite(intro, 0, 0);
  } else if (timer.GetElapsedTime<seconds>() < 2) {
    aar::draw::sprite(title, 0, 0);
  } else {
    aar::draw::clearColor(aar::util::makeColor(0, 0, 0));
    aar::draw::stretchSprite(background, 105, 140, 1070, 680);

    if (frame >= 0 && frame < INTRO_FRAMES) {
      aar::draw::stretchSprite(images[frame], 105, 120, 1070, 660);
    }
  }
}
