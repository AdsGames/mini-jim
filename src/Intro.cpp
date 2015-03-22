#include "Intro.h"
#include "tools.h"

Intro::Intro()
{
  // Load background
  background = load_bitmap( "images/opening/background.png", NULL);
  intro = load_bitmap( "images/opening/intro.png", NULL);
  title = load_bitmap( "images/opening/title.png", NULL);
  introSound = load_sample( "sounds/introSound.wav");

  string fileName;
  for( int i = 0; i < 81; i++){
    fileName = "images/opening/opening" + convertIntToString( i) + ".png";
    images[i] = load_bitmap( fileName.c_str(), NULL);
  }

  buffer = create_bitmap( 1280, 960);
}

void Intro::update()
{
  // Wait 2 seconds then go to the menu
  set_next_state(STATE_MENU);
}

void Intro::draw()
{
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

Intro::~Intro()
{
  // Clear memory
  for( int i = 0; i < 81; i++){
    if(images[i]){
      destroy_bitmap( images[i]);
    }
  }
  destroy_bitmap( background);
  destroy_bitmap( title);
  destroy_bitmap( intro);
  destroy_sample( introSound);
}
