#include "Init.h"

Init::Init()
{
  // Set window title
  set_window_title("Error");

  int width;
  int height;
  get_desktop_resolution(&width,&height);

  set_gfx_mode( GFX_AUTODETECT, width,height , 0, 0);

  set_window_title("Tile Engine");
}

void Init::update()
{
  // Change to splash screen
  set_next_state(STATE_INTRO);
}

void Init::draw()
{

}

Init::~Init()
{

}
