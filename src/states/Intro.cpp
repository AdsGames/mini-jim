#include "./Intro.h"

#include <string>

#include "../Controls.h"
#include "../globals.h"

void Intro::init() {
  background =
      asw::assets::load_texture("assets/images/opening/background.png");
  intro = asw::assets::load_texture("assets/images/opening/intro.png");
  title = asw::assets::load_texture("assets/images/opening/title.png");
  introSound = asw::assets::load_music("assets/sounds/introSound.wav");

  current_frame = nullptr;
  loaded_frame = -1;
  frame = 0;
  sound_played = false;

  timer.start();
}

void Intro::update(float dt) {
  frame = (timer.getElapsedTime<std::chrono::milliseconds>() - 3000) / 100;

  if (frame >= 0 && !sound_played) {
    asw::sound::play_music(introSound);
    sound_played = true;
  }

  if (frame >= INTRO_FRAMES || asw::input::get_keyboard().any_pressed ||
      controls::any_controller_skip()) {
    manager.set_next_scene(ProgramState::Menu);
    return;
  }

  // Load the new frame, the old one is released with it
  if (frame >= 0 && frame != loaded_frame) {
    current_frame = asw::assets::load_texture("assets/images/opening/opening" +
                                              std::to_string(frame) + ".png");
    loaded_frame = frame;
  }
}

void Intro::cleanup() {
  // Stop the intro sound when skipped, it plays on the music track
  asw::sound::stop_music();

  // Scene stays registered, so release its textures once it is done
  intro = nullptr;
  title = nullptr;
  background = nullptr;
  current_frame = nullptr;
  loaded_frame = -1;

  asw::scene::Scene<ProgramState>::cleanup();
}

void Intro::draw() {
  // Intro stuffs
  if (timer.getElapsedTime<std::chrono::seconds>() < 1) {
    asw::draw::sprite(intro, asw::Vec2f(0, 0));
  } else if (timer.getElapsedTime<std::chrono::seconds>() < 2) {
    asw::draw::sprite(title, asw::Vec2f(0, 0));
  } else {
    asw::draw::clear_color(asw::Color(0, 0, 0));
    asw::draw::stretch_sprite(background, asw::Quadf(105, 140, 1070, 680));

    if (current_frame) {
      asw::draw::stretch_sprite(current_frame, asw::Quadf(105, 120, 1070, 660));
    }
  }
}
