#include "Intro.h"
#include "utility/tools.h"

#include <string>
#include <vector>

#include "globals.h"
#include "utility/KeyListener.h"
#include "utility/tools.h"

void Intro::init(aar::Window* window) {
  background = aar::load::bitmap("assets/images/opening/background.png");
  intro = aar::load::bitmap("assets/images/opening/intro.png");
  title = aar::load::bitmap("assets/images/opening/title.png");
  introSound = aar::load::sample("assets/sounds/introSound.wav");

  for (int i = 0; i < INTRO_FRAMES; i++) {
    images[i] = aar::load::bitmap(std::string("assets/images/opening/opening" +
                                              std::to_string(i) + ".png")
                                      .c_str());
  }

  timer.Start();
  highcolor_fade_in(intro, 32);
  frame = 0;
  sound_played = false;
}

Intro::~Intro() {
  for (int i = 0; i < INTRO_FRAMES; i++)
    aar::load::destroyBitmap(images[i]);

  aar::load::destroyBitmap(background);
  aar::load::destroyBitmap(title);
  aar::load::destroyBitmap(intro);
  aar::load::destroySample(introSound);
}

void Intro::update(StateEngine& engine) {
  // poll_joystick();
  frame = (timer.GetElapsedTime<milliseconds>() - 3000) / 100;

  if (frame >= 0 && !sound_played) {
    aar::sound::play(introSound, 255, 128, 1000, 0);
    sound_played = true;
  }

  if (frame >= INTRO_FRAMES || KeyListener::anyKeyPressed) {
    setNextState(engine, StateEngine::STATE_MENU);
    highcolor_fade_out(64);
  }
}

void Intro::draw(aar::Renderer* buffer) {
  // Intro stuffs
  if (timer.GetElapsedTime<seconds>() < 1) {
    aar::draw::sprite(intro, 0, 0);
  } else if (timer.GetElapsedTime<seconds>() < 2) {
    aar::draw::sprite(title, 0, 0);
  } else {
    aar::draw::clearColor(aar::util::makeColor(0, 0, 0, 255));
    aar::draw::stretchSprite(background, 105, 140, 1070, 680);

    if (frame >= 0 && frame < INTRO_FRAMES) {
      aar::draw::stretchSprite(images[frame], 105, 120, 1070, 660);
    }
  }
}
