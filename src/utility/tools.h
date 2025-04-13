#pragma once

// Collision
extern auto collisionAny(int xMin1,
                         int xMax1,
                         int xMin2,
                         int xMax2,
                         int yMin1,
                         int yMax1,
                         int yMin2,
                         int yMax2) -> bool;
extern auto collisionBottom(int yMin1, int yMax1, int yMax2) -> bool;
extern auto collisionTop(int yMin1, int yMax1, int yMin2) -> bool;
extern auto collisionRight(int xMin1, int xMax1, int xMin2) -> bool;
extern auto collisionLeft(int xMin1, int xMax1, int xMax2) -> bool;

// Random number generator
extern auto random(int low, int high) -> int;
