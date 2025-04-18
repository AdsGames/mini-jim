#include "./Init.h"

#include <asw/asw.h>

#include "../globals.h"

#include "../TileTypeLoader.h"

void Init::init() {
  asw::display::setTitle("Setting up");

  TileTypeLoader::loadTypes("assets/levels/tiles.json");
  asw::display::setIcon("assets/icon.ico");

  asw::display::setTitle("Mini Jim");
}

void Init::update(float dt) {
  sceneManager.setNextScene(ProgramState::Intro);
}

void Init::draw() {
  asw::draw::clearColor(asw::util::makeColor(0, 0, 0));
}
