#ifndef TILE_H
#define TILE_H

#include <allegro.h>
#include <string>
#include <vector>

#include "TileType.h"

class Tile {
  public:
    explicit Tile (short type);
    Tile (short type, int x, int y);
    ~Tile();

    int getX() const;
    int getY() const;

    int getWidth();
    int getHeight();

    void setX (int newX);
    void setY (int newY);

    short getType();
    std::string getName();
    std::string getTypeStr();
    bool containsAttribute (int newAttribute);

    void setType (short type);
    void setType(std::string type);

    void draw (BITMAP *buffer, int xOffset, int yOffset, int frame);

  private:
    int x, y;
    std::vector<int> attribute;

    TileType *t_type;
};

#endif
