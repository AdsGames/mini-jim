#include "Init.h"

#include <allegro.h>

#include "globals.h"
#include "utility/tools.h"

#include "TileTypeLoader.h"

Init::Init() {
  set_window_title("Setting up");

  set_display_switch_mode(SWITCH_BACKGROUND);

  // Setup number generator
  srand(time(nullptr));

  TileTypeLoader::LoadTypes("data/tiles.xml");

  set_window_title("Mini Jim");
}

void Init::update(StateEngine& engine) {
  setNextState(engine, StateEngine::STATE_INTRO);
}

void Init::draw(BITMAP* buffer) {
  clear_to_color(buffer, 0x000000);
}

Init::~Init() {}
