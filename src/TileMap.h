#pragma once

#include <asw/asw.h>
#include <asw/util/Timer.h>
#include <string>
#include <vector>

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

  void draw(const asw::Quad<float>& camera,
            float destX,
            float destY,
            int layer);

  void drawShadows(const asw::Quad<float>& camera, float destX, float destY);

  void drawLights(const asw::Quad<float>& camera, float destX, float destY);

  bool load(const std::string& path);

  Tile* find_tile_type(short type, int layer);
  std::vector<Tile*> get_tiles_in_range(const asw::Quad<float>& range);

 private:
  void load_layer(const std::vector<int>& data, std::vector<Tile>& t_map);

  void generate_shadow_map();

  void generate_light_map();

  void draw_layer(std::vector<Tile>& t_map,
                  const asw::Quad<float>& camera,
                  float destX = 0,
                  float destY = 0);

  std::vector<Tile> mapTiles;
  std::vector<Tile> mapTilesBack;
  std::vector<short> shadowMap;

  std::array<asw::Texture, 8> shadowTextures;

  int width{0};
  int height{0};
  bool lighting{false};

  float frame_timer{0.0F};

  LightLayer lightLayer;
};
