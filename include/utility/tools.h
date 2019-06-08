#ifndef TOOLS_H
#define TOOLS_H

#include <allegro.h>
#include <loadpng.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

// Key or joy button pressed
extern bool key_down();
extern bool button_down();

//Collision
extern bool collisionAny (int xMin1, int xMax1, int xMin2, int xMax2, int yMin1, int yMax1, int yMin2, int yMax2);
extern bool collisionBottom (int yMin1, int yMax1, int yMax2);
extern bool collisionTop (int yMin1, int yMax1, int yMin2);
extern bool collisionRight (int xMin1, int xMax1, int xMin2);
extern bool collisionLeft (int xMin1, int xMax1, int xMax2);

//Random number generator
extern int random (int newLowest, int newHighest);

// Fade in and out
extern void highcolor_fade_in (BITMAP *bmp_orig, int speed);
extern void highcolor_fade_out (int speed);

// Error reporting
extern void abort_on_error (std::string path);

// Load image
extern BITMAP *load_png_ex (std::string path);

// Load ogg
extern SAMPLE *load_ogg_ex (std::string path);

// Load sample
extern SAMPLE *load_sample_ex (std::string path);

// Load font
extern FONT *load_font_ex (std::string path);


#endif // TOOLS_H
