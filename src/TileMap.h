#ifndef TILEMAP_H
#define TILEMAP_H

#include <asw/asw.h>
#include <asw/util/Timer.h>
#include <string>
#include <vector>

#include "./Tile.h"

class TileMap {
 public:
  explicit TileMap(const std::string& file = "");

  std::vector<Tile> mapTiles;
  std::vector<Tile> mapTilesBack;

  int getWidth() const;
  int getHeight() const;
  int getFrame();

  bool hasLighting() const;
  void toggleLights();

  void draw(int x,
            int y,
            int width,
            int height,
            int destX = 0,
            int destY = 0,
            int layer = 0);

  void create(int width, int height);
  bool load(const std::string& file);
  void save(const std::string& file);

  Tile* get_tile_at(int s_x, int s_y, int layer);
  Tile* find_tile_type(int type, int layer);
  std::vector<Tile*> get_tiles_in_range(int x_1, int x_2, int y_1, int y_2);

 private:
  void load_layer(std::ifstream& file, std::vector<Tile>& t_map);
  void save_layer(std::ofstream& file, std::vector<Tile>& t_map);
  void draw_layer(std::vector<Tile>& t_map,
                  int x,
                  int y,
                  int width,
                  int height,
                  int destX = 0,
                  int destY = 0);

  int width, height;
  bool lighting;

  Timer frame_timer;
};

#endif
