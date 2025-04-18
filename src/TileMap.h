#pragma once

#include <asw/asw.h>
#include <asw/util/Timer.h>
#include <string>
#include <vector>

#include "./Tile.h"

class TileMap {
 public:
  TileMap() { frame_timer.start(); };

  std::vector<Tile> mapTiles;
  std::vector<Tile> mapTilesBack;

  int getWidth() const;
  int getHeight() const;
  int getFrame();

  bool hasLighting() const;

  void draw(const asw::Quad<float>& camera,
            int destX = 0,
            int destY = 0,
            int layer = 0);

  bool load(const std::string& path);

  Tile* find_tile_type(int type, int layer);
  std::vector<Tile*> get_tiles_in_range(const asw::Quad<float>& range);

 private:
  void load_layer(const std::vector<int>& data, std::vector<Tile>& t_map);
  void draw_layer(std::vector<Tile>& t_map,
                  const asw::Quad<float>& camera,
                  int destX = 0,
                  int destY = 0);

  int width{0};
  int height{0};
  bool lighting{false};

  Timer frame_timer{};
};
