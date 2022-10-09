#include "Init.h"

#include <asw/asw.h>

#include "globals.h"
#include "utility/tools.h"

#include "TileTypeLoader.h"

void Init::init() {
  asw::display::setTitle("Setting up");

  TileTypeLoader::LoadTypes("assets/data/tiles.xml");

  asw::display::setTitle("Mini Jim");
}

void Init::cleanup() {}

void Init::update() {
  setNextState(StateEngine::STATE_INTRO);
}

void Init::draw() {
  asw::draw::clearColor(asw::util::makeColor(0, 0, 0));
}
