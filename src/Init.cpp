#include "Init.h"

#include "./lib/aar/aar.h"

#include "globals.h"
#include "utility/tools.h"

#include "TileTypeLoader.h"

void Init::init() {
  aar::display::setTitle("Setting up");

  TileTypeLoader::LoadTypes("assets/data/tiles.xml");

  aar::display::setTitle("Mini Jim");
}

void Init::cleanup() {}

void Init::update(StateEngine& engine) {
  setNextState(engine, StateEngine::STATE_INTRO);
}

void Init::draw() {
  aar::draw::clearColor(aar::util::makeColor(0, 0, 0));
}
