#include "./Intro.h"

#include <string>
#include <vector>

#include "../globals.h"
#include "../utility/tools.h"

void Intro::init() {
  background = asw::assets::loadTexture("assets/images/opening/background.png");
  intro = asw::assets::loadTexture("assets/images/opening/intro.png");
  title = asw::assets::loadTexture("assets/images/opening/title.png");
  introSound = asw::assets::loadSample("assets/sounds/introSound.wav");

  for (int i = 0; i < INTRO_FRAMES; i++) {
    images[i] = asw::assets::loadTexture("assets/images/opening/opening" +
                                         std::to_string(i) + ".png");
  }

  timer.start();
}

void Intro::update(float dt) {
  frame = (timer.getElapsedTime<std::chrono::milliseconds>() - 3000) / 100;

  if (frame >= 0 && !sound_played) {
    asw::sound::play(introSound, 255, 128, 0);
    sound_played = true;
  }

  if (frame >= INTRO_FRAMES || asw::input::keyboard.anyPressed) {
    sceneManager.setNextScene(ProgramState::Menu);
  }
}

void Intro::draw() {
  // Intro stuffs
  if (timer.getElapsedTime<std::chrono::seconds>() < 1) {
    asw::draw::sprite(intro, asw::Vec2<float>(0, 0));
  } else if (timer.getElapsedTime<std::chrono::seconds>() < 2) {
    asw::draw::sprite(title, asw::Vec2<float>(0, 0));
  } else {
    asw::draw::clearColor(asw::util::makeColor(0, 0, 0));
    asw::draw::stretchSprite(background, asw::Quad<float>(105, 140, 1070, 680));

    if (frame >= 0 && frame < INTRO_FRAMES) {
      asw::draw::stretchSprite(images[frame],
                               asw::Quad<float>(105, 120, 1070, 660));
    }
  }
}
