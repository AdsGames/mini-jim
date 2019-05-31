#ifndef TILEMAP_H
#define TILEMAP_H

#define FRAME_MILASECONDS 100

#include <allegro.h>
#include <loadpng.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#include "globals.h"
#include "utility/tools.h"

#include "tile.h"

using namespace std;

class tileMap {
  public:
    tileMap (std::string fileName);

    std::vector<tile> mapTiles;
    std::vector<tile> mapTilesBack;

    int x;
    int y;

    int width;
    int height;

    long getFrame();

    void load_images();
    void draw_map (BITMAP *tempSprite);
    void draw_map (BITMAP *tempSprite, int newX, int newY);
    void load (std::string fileName);
    void save(std::string file);

    tile *get_tile_at (int x, int y, int layer);

    ~tileMap();
  private:
    static volatile long frame;
    static void change_frame();
};

#endif
