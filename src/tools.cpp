#include "tools.h"
#include "globals.h"

//Iterates through the number of buttons in a joystick and returns true if any keys are pressed
bool keyboard_keypressed(){
    bool keypressed=false;
    for(int i=0; i<125; i++)
        if(key[i])keypressed=true;
    return keypressed;

}
//Iterates through the number of buttons in a joystick and returns true if any buttons are pressed
bool joy_buttonpressed(){
    bool buttonpressed=false;
    for(int i=0; i<joy[0].num_buttons; i++)
        if(joy[0].button[i].b)buttonpressed=true;
    return buttonpressed;
}
//Collision
bool collisionAny(int xMin1, int xMax1, int xMin2, int xMax2, int yMin1, int yMax1, int yMin2, int yMax2){
  if (xMin1 < xMax2 && yMin1 < yMax2 && xMin2 < xMax1 && yMin2 < yMax1){
    return true;
  }
  return false;
}
bool collisionBottom(int yMin1, int yMax1, int yMin2, int yMax2){
  if(yMin1 < yMax2 && yMax1 > yMax2){
    return true;
  }
  return false;
}
bool collisionTop(int yMin1, int yMax1, int yMin2, int yMax2){
  if(yMin2 < yMax1 && yMin1 < yMin2){
    return true;
  }
  return false;
}
bool collisionRight(int xMin1, int xMax1, int xMin2, int xMax2){
  if(xMin2 < xMax1 && xMin1 < xMin2){
    return true;
  }
  return false;
}
bool collisionLeft(int xMin1, int xMax1, int xMin2, int xMax2){
  if(xMin1 < xMax2 && xMax1 > xMax2){
    return true;
  }
  return false;
}

//Checks if file exists
bool fexists(const char *filename){
  ifstream ifile(filename);
  return ifile;
}

//Random number generator. Use int random(lowest,highest);
int random(int newLowest, int newHighest){
  int lowest = newLowest, highest = newHighest;
  int range = (highest - lowest) + 1;
  int randomNumber = lowest+int(range*rand()/(RAND_MAX + 1.0));
  return randomNumber;
}

//Convert int to string
string convertIntToString(int number){
  stringstream ss;
  ss << number;
  return ss.str();
}

//Convert double to string
string convertDoubleToString(double number){
  stringstream ss;
  ss << number;
  return ss.str();
}

//Convert bool to string
string convertBoolToString(bool boolean){
  stringstream ss;
  ss << boolean;
  return ss.str();
}

// Set next state
void set_next_state( int newState )
{
    //If the user doesn't want to exit
    if( nextState != STATE_EXIT )
    {
        //Set the next state
        nextState = newState;
    }
}

// Fade in
void highcolor_fade_in(BITMAP* bmp_orig, int speed){
  BITMAP* bmp_buff = create_bitmap(SCREEN_W,SCREEN_H);
  BITMAP* str_orig = create_bitmap( SCREEN_W, SCREEN_H);
  stretch_sprite( str_orig, bmp_orig, 0, 0, SCREEN_W, SCREEN_H);

  if ( speed<=0)
    speed=16;

  for(int a=0; a<256; a+=speed){
    clear( bmp_buff);
    set_trans_blender( 0, 0, 0, a);
    draw_trans_sprite( bmp_buff, str_orig, 0, 0);
    vsync();
    stretch_sprite( screen, bmp_buff, 0, 0,  SCREEN_W, SCREEN_H);
  }
  stretch_sprite( screen, str_orig, 0, 0,  SCREEN_W, SCREEN_H);
}

// Fade out
void highcolor_fade_out(int speed){
  BITMAP* bmp_buff = create_bitmap(SCREEN_W,SCREEN_H);
  BITMAP *bmp_orig = create_bitmap( SCREEN_W, SCREEN_H);
  blit(screen,bmp_orig,0,0,0,0,SCREEN_W,SCREEN_H);

  if ( speed<=0)
    speed=16;

  for( int a=255 - speed; a>0; a-=speed){
    clear( bmp_buff);
    set_trans_blender( 0, 0, 0, a);
    draw_trans_sprite( bmp_buff, bmp_orig, 0, 0);
    vsync();
    stretch_sprite( screen, bmp_buff, 0, 0,  SCREEN_W, SCREEN_H);
  }
  destroy_bitmap( bmp_orig);
  rectfill( screen, 0, 0,  SCREEN_W, SCREEN_H, makecol(0,0,0));
}

/*
 *  ERROR REPORTING
 */
void abort_on_error(const char *message){
	 if (screen != NULL){
	    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	 }
	 allegro_message("%s.\n %s\n", message, allegro_error);
	 exit(-1);
}

