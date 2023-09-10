#include "tools.h"
#include "../globals.h"

#include <random>
#include <time.h>

// Random device
std::mt19937 rng(time(nullptr));

// Collision
auto collisionAny(int xMin1,
                  int xMax1,
                  int xMin2,
                  int xMax2,
                  int yMin1,
                  int yMax1,
                  int yMin2,
                  int yMax2) -> bool {
  return (xMin1 < xMax2 && yMin1 < yMax2 && xMin2 < xMax1 && yMin2 < yMax1);
}

auto collisionTop(int yMin1, int yMax1, int yMin2) -> bool {
  return (yMin2 < yMax1 && yMin1 < yMin2);
}

auto collisionRight(int xMin1, int xMax1, int xMin2) -> bool {
  return (xMin2 < xMax1 && xMin1 < xMin2);
}

auto collisionLeft(int xMin1, int xMax1, int xMax2) -> bool {
  return (xMin1 < xMax2 && xMax1 > xMax2);
}

// Random number generator. Use int random(lowest,highest);
auto random(int low, int high) -> int {
  std::uniform_int_distribution<int> dist6(
      low, high);  // distribution in range [1, 6]
  return dist6(rng);
}
