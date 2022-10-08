#ifndef TOOLS_H
#define TOOLS_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "../lib/aar/aar.h"

// Collision
extern bool collisionAny(int xMin1,
                         int xMax1,
                         int xMin2,
                         int xMax2,
                         int yMin1,
                         int yMax1,
                         int yMin2,
                         int yMax2);
extern bool collisionBottom(int yMin1, int yMax1, int yMax2);
extern bool collisionTop(int yMin1, int yMax1, int yMin2);
extern bool collisionRight(int xMin1, int xMax1, int xMin2);
extern bool collisionLeft(int xMin1, int xMax1, int xMax2);

// Random number generator
extern int random(int low, int high);

// Fade in and out
extern void highcolor_fade_in(aar::Texture* bmp_orig, int speed);
extern void highcolor_fade_out(int speed);

#endif  // TOOLS_H
