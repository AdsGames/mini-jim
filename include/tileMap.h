#ifndef TILEMAP_H
#define TILEMAP_H

#include <allegro.h>
#include <string>
#include <vector>

#include "tile.h"
#include "Timer.h"

class TileMap {
  public:
    TileMap (std::string file = "");
    ~TileMap();

    std::vector<tile> mapTiles;
    std::vector<tile> mapTilesBack;

    int x;
    int y;

    int width;
    int height;

    int getFrame();

    void draw (BITMAP *buffer);
    void draw (BITMAP *buffer, int x, int y);

    bool load (std::string file);
    void save (std::string file);

    tile *get_tile_at (int x, int y, int layer);

  private:
    void save_layer (std::string file, std::vector<tile> *layer);
    void draw_layer (BITMAP *buffer, std::vector<tile> *layer, int x, int y);

    Timer frame_timer;
};

#endif
