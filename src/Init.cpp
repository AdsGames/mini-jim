#include "Init.h"

#include <allegro.h>

#include "globals.h"
#include "utility/tools.h"

Init::Init() {
  set_window_title ("Setting up");
  set_gfx_mode (GFX_AUTODETECT_WINDOWED, 1280, 960, 0, 0);
  set_window_title ("Mini Jim");
}

void Init::update() {
  set_next_state (STATE_INTRO);
}

void Init::draw() {

}

Init::~Init() {

}
