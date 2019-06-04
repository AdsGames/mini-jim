#ifndef TILE_H
#define TILE_H

#include <allegro.h>
#include <string>
#include <vector>

#include "TileType.h"

class tile {
  public:
    tile (int type);
    tile (int type, int x, int y);
    ~tile();

    int getX() const;
    int getY() const;

    int getWidth();
    int getHeight();

    void setX (int newX);
    void setY (int newY);

    int getType();
    std::string getName();
    bool containsAttribute (int newAttribute);

    void setType (int type);
    void draw_tile (BITMAP *buffer, int xOffset, int yOffset, int frame);

  private:
    int x, y;
    int width, height;
    std::vector<int> attribute;

    TileType *t_type;
};

#endif
