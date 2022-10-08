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
