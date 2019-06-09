#ifndef TILEMAP_H
#define TILEMAP_H

#include <allegro.h>
#include <string>
#include <vector>

#include "Tile.h"
#include "utility/Timer.h"

class TileMap {
  public:
    explicit TileMap (const std::string &file = "");
    ~TileMap();

    std::vector<Tile> mapTiles;
    std::vector<Tile> mapTilesBack;

    int getWidth() const;
    int getHeight() const;
    int getFrame();

    bool hasLighting() const;
    void toggleLights();

    void draw (BITMAP *buffer, int x, int y, int layer = 0);

    void create(int width, int height);
    bool load (const std::string &file);
    void save (const std::string &file);

    Tile *get_tile_at (int x, int y, int layer);
    Tile *find_tile_type (int type, int layer);
    std::vector<Tile *> get_tiles_in_range (int x_1, int x_2, int y_1, int y_2);

  private:
    void load_layer(std::ifstream &file, std::vector<Tile> &t_map);
    void save_layer(std::ofstream &file, std::vector<Tile> &t_map);
    void draw_layer (BITMAP *buffer, std::vector<Tile> &t_map, int x, int y);

    int width, height;
    bool lighting;

    Timer frame_timer;
};

#endif
