#ifndef TILEMAP_H
#define TILEMAP_H

#include <allegro.h>
#include <string>
#include <vector>

#include "Tile.h"
#include "utility/Timer.h"

class TileMap {
  public:
    TileMap (std::string file = "");
    ~TileMap();

    std::vector<tile> mapTiles;
    std::vector<tile> mapTilesBack;

    int getWidth() const;
    int getHeight() const;
    int getFrame();

    void draw (BITMAP *buffer, int x, int y);

    void create(int width, int height);
    bool load (std::string file);
    bool load_txt (std::string file);
    void save (std::string file);

    tile *get_tile_at (int x, int y, int layer);
    tile *find_tile_type (int type, int layer);
    std::vector<tile*> get_tiles_in_range (int x_1, int x_2, int y_1, int y_2);

  private:
    void load_layer(std::ifstream &file, std::vector<tile> &t_map);
    void save_layer(std::ofstream &file, std::vector<tile> &t_map);
    void draw_layer (BITMAP *buffer, std::vector<tile> *layer, int x, int y);

    int width;
    int height;

    Timer frame_timer;
};

#endif
