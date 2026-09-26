#include "./Init.h"

#include <asw/asw.h>

#include "../globals.h"

#include "../TileTypeLoader.h"

void Init::init() {
  asw::display::set_title("Setting up");

  TileTypeLoader::loadTypes("assets/levels/tiles.json");
  asw::display::set_icon("assets/icon.ico");

  asw::display::set_title("Mini Jim");
}

void Init::update(float dt) {
  manager.set_next_scene(ProgramState::Intro);
}

void Init::draw() {
  asw::draw::clear_color(asw::Color(0, 0, 0));
}
