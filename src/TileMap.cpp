#include "TileMap.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>

#include "globals.h"

// Get width
auto TileMap::getWidth() const -> int {
  return width * 64;
}

// Get height
auto TileMap::getHeight() const -> int {
  return height * 64;
}

// Get frame
auto TileMap::getFrame() -> int {
  return int(frame_timer.getElapsedTime<std::chrono::milliseconds>() / 100) % 8;
}

// Has lighting enabled
auto TileMap::hasLighting() const -> bool {
  return lighting;
}

// Toggle lighting effects
void TileMap::toggleLights() {
  lighting = !lighting;
}

void TileMap::create(int width, int height) {
  mapTiles.clear();
  mapTilesBack.clear();
  this->width = width;
  this->height = height;
  this->lighting = false;

  for (int t = 0; t < height; t++) {
    for (int i = 0; i < width; i++) {
      mapTiles.push_back(Tile(0, i * 64, t * 64));
      mapTilesBack.push_back(Tile(0, i * 64, t * 64));
    }
  }
}

void TileMap::load_layer(const std::vector<int>& data,
                         std::vector<Tile>& t_map) {
  int position = 0;

  for (unsigned long i = 0; i < data.size(); i++) {
    const auto id = data[i];

    if (data[i] != 0) {
      // Tiled adds 1 to the id
      t_map.emplace_back(id - 1, (position % width) * 64,
                         (position / width) * 64);
    }
    position++;
  }
}

auto TileMap::load(const std::string& path) -> bool {
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
  load_layer(foreground, mapTiles);

  const std::vector<int> background = doc["layers"][0]["data"];
  load_layer(background, mapTilesBack);

  file.close();

  return true;
}

// Get tile at
auto TileMap::get_tile_at(int s_x, int s_y, int layer) -> Tile* {
  std::vector<Tile>* ttm = (layer == 1) ? &mapTiles : &mapTilesBack;

  for (auto& t : *ttm) {
    if (t.getBoundingBox().contains(s_x, s_y)) {
      return &t;
    }
  }

  return nullptr;
}

// Find tile type
auto TileMap::find_tile_type(int type, int layer) -> Tile* {
  std::vector<Tile>* ttm = (layer == 1) ? &mapTiles : &mapTilesBack;

  for (auto& t : *ttm) {
    if (t.getType() == type) {
      return &t;
    }
  }

  return nullptr;
}

// Get tile at
std::vector<Tile*> TileMap::get_tiles_in_range(int x_1,
                                               int x_2,
                                               int y_1,
                                               int y_2) {
  std::vector<Tile*> ranged_map;

  for (auto& t : mapTiles) {
    if (t.getType() != 0 && t.getBoundingBox().collides(asw::Quad<float>(
                                x_1, y_1, x_2 - x_1, y_2 - y_1))) {
      ranged_map.push_back(&t);
    }
  }

  return ranged_map;
}

// Draw a layer
void TileMap::draw_layer(std::vector<Tile>& t_map,
                         int x,
                         int y,
                         int width,
                         int height,
                         int destX,
                         int destY) {
  int const frame = getFrame();

  for (auto& t : t_map) {
    if ((t.getX() + t.getWidth() >= x) && (t.getX() < x + width) &&
        (t.getY() + t.getHeight() >= y) && (t.getY() < y + height)) {
      t.draw(x - destX, y - destY, frame);
    }
  }
}

// Draw at position
void TileMap::draw(int x,
                   int y,
                   int width,
                   int height,
                   int destX,
                   int destY,
                   int layer) {
  if (layer == 0 || layer == 1) {
    draw_layer(mapTilesBack, x, y, width, height, destX, destY);
  }

  // Draw semi-transparent buffer
  if (layer == 0) {
    SDL_SetRenderDrawBlendMode(asw::display::renderer, SDL_BLENDMODE_BLEND);
    asw::draw::rectFill(asw::Quad<float>(0, 0, width, height),
                        asw::util::makeColor(0, 0, 0, 128));
    SDL_SetRenderDrawBlendMode(asw::display::renderer, SDL_BLENDMODE_NONE);
  }

  if (layer == 0 || layer == 2) {
    draw_layer(mapTiles, x, y, width, height, destX, destY);
  }
}
