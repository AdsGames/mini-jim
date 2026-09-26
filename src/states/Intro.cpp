#include "./Intro.h"

#include <string>
#include <vector>

#include "../globals.h"

void Intro::init() {
  background = asw::assets::load_texture("assets/images/opening/background.png");
  intro = asw::assets::load_texture("assets/images/opening/intro.png");
  title = asw::assets::load_texture("assets/images/opening/title.png");
  introSound = asw::assets::load_sample("assets/sounds/introSound.wav");

  for (int i = 0; i < INTRO_FRAMES; i++) {
    images[i] = asw::assets::load_texture("assets/images/opening/opening" +
                                         std::to_string(i) + ".png");
  }

  timer.start();
}

void Intro::update(float dt) {
  frame = (timer.getElapsedTime<std::chrono::milliseconds>() - 3000) / 100;

  if (frame >= 0 && !sound_played) {
    asw::sound::play(introSound);
    sound_played = true;
  }

  if (frame >= INTRO_FRAMES || asw::input::get_keyboard().any_pressed) {
    manager.set_next_scene(ProgramState::Menu);
  }
}

void Intro::draw() {
  // Intro stuffs
  if (timer.getElapsedTime<std::chrono::seconds>() < 1) {
    asw::draw::sprite(intro, asw::Vec2<float>(0, 0));
  } else if (timer.getElapsedTime<std::chrono::seconds>() < 2) {
    asw::draw::sprite(title, asw::Vec2<float>(0, 0));
  } else {
    asw::draw::clear_color(asw::Color(0, 0, 0));
    asw::draw::stretch_sprite(background, asw::Quad<float>(105, 140, 1070, 680));

    if (frame >= 0 && frame < INTRO_FRAMES) {
      asw::draw::stretch_sprite(images[frame],
                               asw::Quad<float>(105, 120, 1070, 660));
    }
  }
}
