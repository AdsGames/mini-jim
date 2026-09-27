#include "TileMap.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>

#include "TileTypeLoader.h"
#include "globals.h"

namespace {
// Light radius in pixels for each light level of a tile
constexpr float LIGHT_RADIUS_PER_LEVEL = 64.0F;
}  // namespace

// Get width
int TileMap::getWidth() const {
  return width * 64;
}

// Get height
int TileMap::getHeight() const {
  return height * 64;
}

// Get frame
int TileMap::getFrame() const {
  return static_cast<int>(frame_timer / 100) % 8;
}

// Has lighting enabled
bool TileMap::hasLighting() const {
  return lighting;
}

bool TileMap::load(const std::string& path) {
  // Load shadow textures
  shadowTextures[1] = asw::assets::load_texture(
      "assets/images/blocks/shadows/shadow_bottom_left.png");
  shadowTextures[2] = asw::assets::load_texture(
      "assets/images/blocks/shadows/shadow_top_right.png");
  shadowTextures[3] = asw::assets::load_texture(
      "assets/images/blocks/shadows/shadow_top_left_corner.png");
  shadowTextures[4] = asw::assets::load_texture(
      "assets/images/blocks/shadows/shadow_top_left.png");
  shadowTextures[5] =
      asw::assets::load_texture("assets/images/blocks/shadows/shadow_left.png");
  shadowTextures[6] =
      asw::assets::load_texture("assets/images/blocks/shadows/shadow_top.png");
  shadowTextures[7] =
      asw::assets::load_texture("assets/images/blocks/shadows/shadow_full.png");

  // Set shadow alpha
  for (auto& t : shadowTextures) {
    asw::draw::set_alpha(t, 0.4F);
  }

  // Open file or abort if it does not exist
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file " << path << '\n';
    return false;
  }

  // Create buffer
  nlohmann::json doc = nlohmann::json::parse(file);

  // Check layers
  if (!doc.contains("layers")) {
    std::cerr << "Error: No layers found in file " << path << '\n';
    file.close();
    return false;
  }

  if (doc["layers"].size() != 2) {
    std::cerr << "Error: Invalid number of layers in file " << path << '\n';
    file.close();
    return false;
  }

  // Setup Map
  mapTiles.clear();
  mapTilesBack.clear();
  width = doc["width"];
  height = doc["height"];
  lighting = false;

  // Read properties if they exist
  if (doc.contains("properties")) {
    for (auto& prop : doc["properties"]) {
      if (prop["name"] == "lighting") {
        lighting = prop["value"];
      }
    }
  }

  // Load data into vector
  const std::vector<int> foreground = doc["layers"][1]["data"];
  load_layer(foreground, mapTiles, mapIndex);

  const std::vector<int> background = doc["layers"][0]["data"];
  load_layer(background, mapTilesBack, mapIndexBack);

  file.close();

  // Generate shadow map
  generate_shadow_map();

  // Generate light map
  generate_light_map();

  return true;
}

void TileMap::load_layer(const std::vector<int>& data,
                         std::vector<Tile>& t_map,
                         std::vector<int>& t_index) {
  int position = 0;
  t_index.assign(width * height, -1);

  for (const int i : data) {
    const auto id = i;

    if (i != 0) {
      // Tiled adds 1 to the id
      t_map.emplace_back(id - 1, (position % width) * 64,
                         (position / width) * 64);
      t_index[position] = static_cast<int>(t_map.size()) - 1;
    }
    position++;
  }
}

template <typename Fn>
void TileMap::for_each_tile_in(std::vector<Tile>& t_map,
                               const std::vector<int>& t_index,
                               const asw::Quadf& range,
                               Fn&& fn) {
  // Tiles can reach past their own cell (e.g. chicken), so widen the search
  // by the largest tile footprint, then do the exact check per tile
  const auto& extent = TileTypeLoader::getExtent();

  const float left = range.position.x - (extent.position.x + extent.size.x);
  const float top = range.position.y - (extent.position.y + extent.size.y);
  const float right = range.position.x + range.size.x - extent.position.x;
  const float bottom = range.position.y + range.size.y - extent.position.y;

  const int x_start =
      std::max(0, static_cast<int>(std::floor(left / 64.0F)));
  const int y_start = std::max(0, static_cast<int>(std::floor(top / 64.0F)));
  const int x_end =
      std::min(width - 1, static_cast<int>(std::floor(right / 64.0F)));
  const int y_end =
      std::min(height - 1, static_cast<int>(std::floor(bottom / 64.0F)));

  for (int y = y_start; y <= y_end; y++) {
    for (int x = x_start; x <= x_end; x++) {
      const int idx = t_index[(y * width) + x];
      if (idx == -1) {
        continue;
      }

      auto& t = t_map[idx];
      if (t.getTransform().collides(range)) {
        fn(t);
      }
    }
  }
}

void TileMap::generate_shadow_map() {
  // Create shadow map
  shadowMap.clear();
  shadowMap.resize(width * height, 0);

  // Set dark tiles
  for (auto& t : mapTiles) {
    if (t.containsAttribute(shadow)) {
      const auto st = t.getTransform() / 64;
      const int x = static_cast<int>(std::floor(st.position.x));
      const int y = static_cast<int>(std::floor(st.position.y));
      const int w = static_cast<int>(std::ceil(st.size.x));
      const int h = static_cast<int>(std::ceil(st.size.y));

      for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
          if (x + i >= width || y + j >= height) {
            continue;  // Skip if out of bounds
          }

          shadowMap[((y + j) * width) + (x + i)] = 7;
        }
      }
    }
  }

  // Set neighbour tiles kernel
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      const int idx = (j * width) + i;

      if (shadowMap[idx] == 7) {
        continue;  // Skip if already solid
      }

      // Check left
      if (i > 0 && shadowMap[idx - 1] == 7) {
        shadowMap[idx] += 1;
      }

      // Check top
      if (j > 0 && shadowMap[idx - width] == 7) {
        shadowMap[idx] += 2;
      }

      // Check diagonal
      if (shadowMap[idx] < 3 && i > 0 && j > 0 &&
          shadowMap[idx - width - 1] == 7) {
        shadowMap[idx] += 4;
      }
    }
  }
}

void TileMap::generate_light_map() {
  // Create light map
  lightLayer.clear();

  // Get map area
  for (auto& t : mapTiles) {
    if (t.containsAttribute(light)) {
      const auto& id = t.getType()->GetIDStr();

      LightKind kind = LightKind::Lamp;
      if (id == "fire") {
        kind = LightKind::Fire;
      } else if (id == "element" || id == "toaster_element") {
        kind = LightKind::Heat;
      }

      lightLayer.addPoint(t.getTransform().get_center(),
                          LIGHT_RADIUS_PER_LEVEL *
                              static_cast<float>(t.getType()->GetLightLevel()),
                          kind);
    }
  }
}

// Find tile type
Tile* TileMap::find_tile_type(short type, int layer) {
  std::vector<Tile>* ttm = (layer == 1) ? &mapTiles : &mapTilesBack;

  for (auto& t : *ttm) {
    if (t.getType()->GetID() == type) {
      return &t;
    }
  }

  return nullptr;
}

// Get tile at
std::vector<Tile*> TileMap::get_tiles_in_range(const asw::Quadf& range) {
  std::vector<Tile*> ranged_map;

  for_each_tile_in(mapTiles, mapIndex, range, [&ranged_map](Tile& t) {
    if (t.getType() != nullptr) {
      ranged_map.push_back(&t);
    }
  });

  return ranged_map;
}

void TileMap::update(float deltaTime) {
  frame_timer += deltaTime;
}

// Draw at position
void TileMap::draw(const asw::Quadf& camera,
                   float destX,
                   float destY,
                   int layer) {
  if (layer == 1) {
    draw_layer(mapTilesBack, mapIndexBack, camera, destX, destY);

    // Draw semi-transparent buffer
    SDL_SetRenderDrawBlendMode(asw::display::get_renderer(),
                               SDL_BLENDMODE_BLEND);
    asw::draw::rect_fill(asw::Quadf(0, 0, getWidth(), getHeight()),
                         asw::Color(0, 0, 0, 64));
    SDL_SetRenderDrawBlendMode(asw::display::get_renderer(),
                               SDL_BLENDMODE_NONE);
  }

  if (layer == 2) {
    draw_layer(mapTiles, mapIndex, camera, destX, destY);
  }
}

void TileMap::drawShadows(const asw::Quadf& camera, float destX, float destY) {
  // Only visit cells inside the camera
  const int x_start =
      std::max(0, static_cast<int>(std::floor(camera.position.x / 64.0F)));
  const int y_start =
      std::max(0, static_cast<int>(std::floor(camera.position.y / 64.0F)));
  const int x_end = std::min(
      width,
      static_cast<int>(std::ceil((camera.position.x + camera.size.x) / 64.0F)));
  const int y_end = std::min(
      height,
      static_cast<int>(std::ceil((camera.position.y + camera.size.y) / 64.0F)));

  // Draw shadow map
  for (int y = y_start; y < y_end; y++) {
    for (int x = x_start; x < x_end; x++) {
      const auto kernelIdx = shadowMap[(y * width) + x];
      if (kernelIdx == 0) {
        continue;
      }

      auto position = asw::Vec2f(x * 64.0F - camera.position.x + destX,
                                 y * 64.0F - camera.position.y + destY);
      asw::draw::sprite(shadowTextures[kernelIdx], position);
    }
  }
}

void TileMap::drawLights(const asw::Quadf& camera,
                         float destX,
                         float destY,
                         const std::vector<asw::Vec2f>& halos) {
  // Only dark levels are lit
  if (!lighting) {
    return;
  }

  lightLayer.draw(camera, destX, destY, frame_timer, halos);
}

// Draw a layer
void TileMap::draw_layer(std::vector<Tile>& t_map,
                         const std::vector<int>& t_index,
                         const asw::Quadf& camera,
                         float destX,
                         float destY) {
  int const frame = getFrame();

  for_each_tile_in(t_map, t_index, camera, [&](Tile& t) {
    t.draw(camera.position.x - destX, camera.position.y - destY, frame);
  });
}