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
std::vector<Tile*> TileMap::get_tiles_in_range(const asw::Quad<float>& range) {
  std::vector<Tile*> ranged_map;

  for (auto& t : mapTiles) {
    if (t.getType() != 0 && t.getTransform().collides(range)) {
      ranged_map.push_back(&t);
    }
  }

  return ranged_map;
}

// Draw a layer
void TileMap::draw_layer(std::vector<Tile>& t_map,
                         const asw::Quad<float>& camera,
                         int destX,
                         int destY) {
  int const frame = getFrame();

  for (auto& t : t_map) {
    if (t.getTransform().collides(camera)) {
      t.draw(camera.position.x - destX, camera.position.y - destY, frame);
    }
  }
}

// Draw at position
void TileMap::draw(const asw::Quad<float>& camera,
                   int destX,
                   int destY,
                   int layer) {
  if (layer == 0 || layer == 1) {
    draw_layer(mapTilesBack, camera, destX, destY);
  }

  // Draw semi-transparent buffer
  if (layer == 0) {
    SDL_SetRenderDrawBlendMode(asw::display::renderer, SDL_BLENDMODE_BLEND);
    asw::draw::rectFill(asw::Quad<float>(0, 0, getWidth(), getHeight()),
                        asw::util::makeColor(0, 0, 0, 110));
    SDL_SetRenderDrawBlendMode(asw::display::renderer, SDL_BLENDMODE_NONE);
  }

  if (layer == 0 || layer == 2) {
    draw_layer(mapTiles, camera, destX, destY);
  }
}
