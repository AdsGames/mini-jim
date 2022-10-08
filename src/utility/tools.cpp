#include "tools.h"
#include "../globals.h"

#include <random>

// Random device
std::mt19937 rng(time(nullptr));

// Collision
bool collisionAny(int xMin1,
                  int xMax1,
                  int xMin2,
                  int xMax2,
                  int yMin1,
                  int yMax1,
                  int yMin2,
                  int yMax2) {
  return (xMin1 < xMax2 && yMin1 < yMax2 && xMin2 < xMax1 && yMin2 < yMax1);
}

bool collisionTop(int yMin1, int yMax1, int yMin2) {
  return (yMin2 < yMax1 && yMin1 < yMin2);
}

bool collisionRight(int xMin1, int xMax1, int xMin2) {
  return (xMin2 < xMax1 && xMin1 < xMin2);
}

bool collisionLeft(int xMin1, int xMax1, int xMax2) {
  return (xMin1 < xMax2 && xMax1 > xMax2);
}

// Random number generator. Use int random(lowest,highest);
int random(int low, int high) {
  std::uniform_int_distribution<int> dist6(
      low, high);  // distribution in range [1, 6]
  return dist6(rng);
}

// Fade in
void highcolor_fade_in(aar::Texture* bmp_orig, int speed) {
  // aar::Texture* bmp_buff = create_bitmap(SCREEN_W, SCREEN_H);

  // speed = speed <= 0 ? 16 : speed;

  // for (int a = 0; a < 256; a += speed) {
  //   clear(bmp_buff);
  //   set_trans_blender(0, 0, 0, a);
  //   aar::draw::sprite(bmp_buff, bmp_orig, 0, 0);
  //   vsync();
  //   aar::draw::stretchSprite(screen, bmp_buff, 0, 0, SCREEN_W, SCREEN_H);
  // }

  // aar::load::destroyBitmap(bmp_buff);
  // aar::draw::stretchSprite(screen, bmp_orig, 0, 0, SCREEN_W, SCREEN_H);
}

// Fade out
void highcolor_fade_out(int speed) {
  //   aar::Texture* bmp_buff = create_bitmap(SCREEN_W, SCREEN_H);
  //   aar::Texture* bmp_orig = create_bitmap(SCREEN_W, SCREEN_H);
  //   blit(screen, bmp_orig, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

  //   speed = speed <= 0 ? 16 : speed;

  //   for (int a = 255 - speed; a > 0; a -= speed) {
  //     clear(bmp_buff);
  //     set_trans_blender(0, 0, 0, a);
  //     aar::draw::sprite(bmp_buff, bmp_orig, 0, 0);
  //     vsync();
  //     aar::draw::stretchSprite(screen, bmp_buff, 0, 0, SCREEN_W, SCREEN_H);
  //   }

  //   aar::load::destroyBitmap(bmp_buff);
  //   aar::load::destroyBitmap(bmp_orig);
  //   aar::draw::primRectFill(screen, 0, 0, SCREEN_W, SCREEN_H, makecol(0, 0,
  //   0));
}
