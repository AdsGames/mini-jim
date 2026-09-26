#pragma once

#include <asw/asw.h>
#include <string>
#include <vector>
#include "Timer.h"

#include "./LightLayer.h"
#include "./Tile.h"

class TileMap {
 public:
  TileMap() = default;

  int getWidth() const;
  int getHeight() const;
  int getFrame() const;

  bool hasLighting() const;

  void update(float deltaTime);

  void draw(const asw::Quadf& camera, float destX, float destY, int layer);

  void drawShadows(const asw::Quadf& camera, float destX, float destY);

  // Halos are lights that follow the players
  void drawLights(const asw::Quadf& camera,
                  float destX,
                  float destY,
                  const std::vector<asw::Vec2f>& halos = {});

  bool load(const std::string& path);

  Tile* find_tile_type(short type, int layer);
  std::vector<Tile*> get_tiles_in_range(const asw::Quadf& range);

 private:
  void load_layer(const std::vector<int>& data,
                  std::vector<Tile>& t_map,
                  std::vector<int>& t_index);

  // Call fn for each tile whose bounding box collides with range, in row order
  template <typename Fn>
  void for_each_tile_in(std::vector<Tile>& t_map,
                        const std::vector<int>& t_index,
                        const asw::Quadf& range,
                        Fn&& fn);

  void generate_shadow_map();

  void generate_light_map();

  void draw_layer(std::vector<Tile>& t_map,
                  const std::vector<int>& t_index,
                  const asw::Quadf& camera,
                  float destX = 0,
                  float destY = 0);

  std::vector<Tile> mapTiles;
  std::vector<Tile> mapTilesBack;

  // Per cell index into mapTiles / mapTilesBack, -1 when empty
  std::vector<int> mapIndex;
  std::vector<int> mapIndexBack;

  std::vector<short> shadowMap;

  std::array<asw::Texture, 8> shadowTextures;

  int width{0};
  int height{0};
  bool lighting{false};

  float frame_timer{0.0F};

  LightLayer lightLayer;
};
