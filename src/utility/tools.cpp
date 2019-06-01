#include "utility/tools.h"
#include "globals.h"

#include <logg.h>

//Iterates through the number of buttons in a joystick and returns true if any keys are pressed
bool key_down() {
  for (int i = 0; i < KEY_MAX; i++) {
    if (key[i]) {
      return true;
    }
  }
  return false;
}

//Iterates through the number of buttons in a joystick and returns true if any buttons are pressed
bool button_down() {
  for (int i = 0; i < joy[0].num_buttons; i++) {
    if (joy[0].button[i].b) {
      return true;
    }
  }
  return false;
}

//Collision
bool collisionAny (int xMin1, int xMax1, int xMin2, int xMax2, int yMin1, int yMax1, int yMin2, int yMax2) {
  return (xMin1 < xMax2 && yMin1 < yMax2 && xMin2 < xMax1 && yMin2 < yMax1);
}
bool collisionBottom (int yMin1, int yMax1, int yMax2) {
  return (yMin1 < yMax2 && yMax1 > yMax2);
}
bool collisionTop (int yMin1, int yMax1, int yMin2) {
  return (yMin2 < yMax1 && yMin1 < yMin2);
}
bool collisionRight (int xMin1, int xMax1, int xMin2) {
  return (xMin2 < xMax1 && xMin1 < xMin2);
}
bool collisionLeft (int xMin1, int xMax1, int xMax2) {
  return (xMin1 < xMax2 && xMax1 > xMax2);
}

//Random number generator. Use int random(lowest,highest);
int random (int newLowest, int newHighest) {
  int lowest = newLowest, highest = newHighest;
  int range = (highest - lowest) + 1;
  int randomNumber = lowest + int (range * rand() / (RAND_MAX + 1.0));
  return randomNumber;
}

// Fade in
void highcolor_fade_in (BITMAP *bmp_orig, int speed) {
  BITMAP *bmp_buff = create_bitmap (SCREEN_W, SCREEN_H);

  speed = speed <= 0 ? 16 : speed;

  for (int a = 0; a < 256; a += speed) {
    clear (bmp_buff);
    set_trans_blender (0, 0, 0, a);
    draw_trans_sprite (bmp_buff, bmp_orig, 0, 0);
    vsync();
    stretch_sprite (screen, bmp_buff, 0, 0, SCREEN_W, SCREEN_H);
  }

  destroy_bitmap (bmp_buff);
  stretch_sprite (screen, bmp_orig, 0, 0, SCREEN_W, SCREEN_H);
}

// Fade out
void highcolor_fade_out (int speed) {
  BITMAP *bmp_buff = create_bitmap (SCREEN_W, SCREEN_H);
  BITMAP *bmp_orig = create_bitmap (SCREEN_W, SCREEN_H);
  blit (screen, bmp_orig, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

  speed = speed <= 0 ? 16 : speed;

  for (int a = 255 - speed; a > 0; a -= speed) {
    clear (bmp_buff);
    set_trans_blender (0, 0, 0, a);
    draw_trans_sprite (bmp_buff, bmp_orig, 0, 0);
    vsync();
    stretch_sprite (screen, bmp_buff, 0, 0,  SCREEN_W, SCREEN_H);
  }

  destroy_bitmap (bmp_buff);
  destroy_bitmap (bmp_orig);
  rectfill (screen, 0, 0,  SCREEN_W, SCREEN_H, makecol (0, 0, 0));
}

// Error reporting
void abort_on_error (const char *message) {
  if (screen != NULL) {
    set_gfx_mode (GFX_TEXT, 0, 0, 0, 0);
  }

  allegro_message ("%s.\n %s\n", message, allegro_error);
  exit (-1);
}

// Load image
BITMAP *load_png_ex (const char *path) {
  BITMAP *temp = nullptr;

  if (! (temp = load_png (path, nullptr))) {
    abort_on_error ((std::string ("Cannot find image (") + path + ") \n Please check your files and try again").c_str());
  }

  return temp;
}

// Load ogg
SAMPLE *load_ogg_ex (const char *path) {
  SAMPLE *temp = nullptr;

  if (! (temp = logg_load (path))) {
    abort_on_error ((std::string ("Cannot find music (") + path + ") \n Please check your files and try again").c_str());
  }

  return temp;
}

// Load sample
SAMPLE *load_sample_ex (const char *path) {
  SAMPLE *temp = nullptr;

  if (! (temp = load_sample (path))) {
    abort_on_error ((std::string ("Cannot find sample (") + path + ") \n Please check your files and try again").c_str());
  }

  return temp;
}

// Load font
FONT *load_font_ex (const char *path) {
  FONT *temp = nullptr;

  if (! (temp = load_font (path, nullptr, nullptr))) {
    abort_on_error ((std::string ("Cannot find font (") + path + ") \n Please check your files and try again").c_str());
  }

  return temp;
}

