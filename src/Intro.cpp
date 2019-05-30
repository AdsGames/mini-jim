#include "Intro.h"
#include "tools.h"

#include <loadpng.h>
#include <string>
#include <vector>
#include <string>

#include "globals.h"
#include "tools.h"

Intro::Intro() {
  background = load_png_ex("images/opening/background.png");
  intro = load_png_ex("images/opening/intro.png");
  title = load_png_ex("images/opening/title.png");
  introSound = load_sample_ex("sounds/introSound.wav");

  for (int i = 0; i < INTRO_FRAMES; i++) {
    images[i] = load_png_ex(std::string("images/opening/opening" + std::to_string(i) + ".png").c_str());
  }

  buffer = create_bitmap (SCREEN_W, SCREEN_H);
}

Intro::~Intro() {
  for (int i = 0; i < INTRO_FRAMES; i++)
    destroy_bitmap (images[i]);

  destroy_bitmap (background);
  destroy_bitmap (title);
  destroy_bitmap (intro);
  destroy_sample (introSound);
}


void Intro::update() {
  set_next_state (STATE_MENU);
}

void Intro::draw() {
  // Intro stuffs
  /* highcolor_fade_in( intro, 32);
     rest(3000);
   highcolor_fade_out( 32);
   highcolor_fade_in( title, 32);
     rest(3000);
   highcolor_fade_out( 32);
   rectfill( buffer, 0, 0, SCREEN_W, SCREEN_H, makecol( 0,0,0));
   stretch_sprite( buffer, background, 105, 140, 1070, 680);
   highcolor_fade_in( buffer, 32);

   clear_keybuf();

   // Show background
   play_sample( introSound, 255, 128, 1000, 0);
   for( int i = 0; i < 81; i++){
     poll_joystick();
     if(keyboard_keypressed() || joy_buttonpressed()){
       break;
     }
     rectfill( buffer, 0, 0, 1280, 960, makecol( 0,0,0));
     stretch_sprite( buffer, background, 105, 140, 1070, 680);
     stretch_sprite( buffer, images[i], 105, 120, 1070, 660);
     stretch_sprite( screen, buffer, 0, 0, SCREEN_W, SCREEN_H);
     // Wheres my car?
     rest(100);
   }
  *///  highcolor_fade_out( 64);
}
