#include "Init.h"

#include "./lib/aar/aar.h"

#include "globals.h"
#include "utility/tools.h"

#include "TileTypeLoader.h"

void Init::init(aar::Window* window) {
  // set_window_title("Setting up");

  // set_display_switch_mode(SWITCH_BACKGROUND);

  // Setup number generator
  srand(time(nullptr));

  TileTypeLoader::LoadTypes("assets/data/tiles.xml");

  // set_window_title("Mini Jim");
}

void Init::update(StateEngine& engine) {
  setNextState(engine, StateEngine::STATE_MENU);
}

void Init::draw(aar::Renderer* buffer) {
  aar::draw::clearColor(aar::util::makeColor(0, 0, 0, 255));
}

Init::~Init() {}
