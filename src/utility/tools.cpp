#include "utility/tools.h"
#include "globals.h"

#include <logg.h>
#include <random>

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
int random (int low, int high) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist6(low,high); // distribution in range [1, 6]
  return dist6(rng);
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
void abort_on_error (std::string message) {
  if (screen != NULL) {
    set_gfx_mode (GFX_TEXT, 0, 0, 0, 0);
  }

  allegro_message ("%s.\n %s\n", message.c_str(), allegro_error);
  exit (-1);
}

// Load image
BITMAP *load_png_ex (std::string path) {
  BITMAP *temp = nullptr;

  if (!(temp = load_png (path.c_str(), nullptr)))
    abort_on_error ("Cannot find image (" + path + ") \n Please check your files and try again");

  return temp;
}

// Load ogg
SAMPLE *load_ogg_ex (std::string path) {
  SAMPLE *temp = nullptr;

  if (!(temp = logg_load (path.c_str())))
    abort_on_error ("Cannot find image (" + path + ") \n Please check your files and try again");

  return temp;
}

// Load sample
SAMPLE *load_sample_ex (std::string path) {
  SAMPLE *temp = nullptr;

  if (!(temp = load_sample (path.c_str())))
    abort_on_error ("Cannot find image (" + path + ") \n Please check your files and try again");

  return temp;
}

// Load font
FONT *load_font_ex (std::string path) {
  FONT *temp = nullptr;

  if (!(temp = load_font (path.c_str(), nullptr, nullptr)))
    abort_on_error ("Cannot find image (" + path + ") \n Please check your files and try again");

  return temp;
}

