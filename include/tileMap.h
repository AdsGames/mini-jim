#ifndef TILEMAP_H
#define TILEMAP_H

#include <allegro.h>
#include <string>
#include <vector>

#include "tile.h"
#include "utility/Timer.h"

class TileMap {
  public:
    TileMap (std::string file = "");
    ~TileMap();

    std::vector<tile> mapTiles;
    std::vector<tile> mapTilesBack;

    int width;
    int height;

    int getFrame();

    void draw (BITMAP *buffer, int x, int y);

    bool load (std::string file);
    void save (std::string file);

    tile *get_tile_at (int x, int y, int layer);
    tile *find_tile_type (int type, int layer);
    std::vector<tile*> get_tiles_in_range (int x_1, int x_2, int y_1, int y_2);

  private:
    void save_layer (std::string file, std::vector<tile> *layer);
    void draw_layer (BITMAP *buffer, std::vector<tile> *layer, int x, int y);

    Timer frame_timer;
};

#endif
