#include "TileMap.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>

#include "globals.h"

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
  shadowTextures[1] = asw::assets::loadTexture(
      "assets/images/blocks/shadows/shadow_bottom_left.png");
  shadowTextures[2] = asw::assets::loadTexture(
      "assets/images/blocks/shadows/shadow_top_right.png");
  shadowTextures[3] = asw::assets::loadTexture(
      "assets/images/blocks/shadows/shadow_top_left_corner.png");
  shadowTextures[4] = asw::assets::loadTexture(
      "assets/images/blocks/shadows/shadow_top_left.png");
  shadowTextures[5] =
      asw::assets::loadTexture("assets/images/blocks/shadows/shadow_left.png");
  shadowTextures[6] =
      asw::assets::loadTexture("assets/images/blocks/shadows/shadow_top.png");
  shadowTextures[7] =
      asw::assets::loadTexture("assets/images/blocks/shadows/shadow_full.png");

  // Set shadow alpha
  for (auto& t : shadowTextures) {
    asw::draw::setAlpha(t, 0.4F);
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
  load_layer(foreground, mapTiles);

  const std::vector<int> background = doc["layers"][0]["data"];
  load_layer(background, mapTilesBack);

  file.close();

  // Generate shadow map
  generate_shadow_map();

  // Generate light map
  generate_light_map();

  return true;
}

void TileMap::load_layer(const std::vector<int>& data,
                         std::vector<Tile>& t_map) {
  int position = 0;

  for (const int i : data) {
    const auto id = i;

    if (i != 0) {
      // Tiled adds 1 to the id
      t_map.emplace_back(id - 1, (position % width) * 64,
                         (position / width) * 64);
    }
    position++;
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
      const int x = static_cast<int>(std::floorf(st.position.x));
      const int y = static_cast<int>(std::floorf(st.position.y));
      const int w = static_cast<int>(std::ceilf(st.size.x));
      const int h = static_cast<int>(std::ceilf(st.size.y));

      for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
          if (x + i >= width || y + j >= height) {
            continue;  // Skip if out of bounds
          }

          shadowMap[(y * width + j) + (x + i)] = 7;
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
  lightLayer.setColor(asw::util::makeColor(255, 255, 255, 128));

  // Get map area
  for (auto& t : mapTiles) {
    if (t.containsAttribute(light)) {
      lightLayer.addPoint(t.getTransform().getCenter(),
                          t.getType()->GetLightLevel());
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
std::vector<Tile*> TileMap::get_tiles_in_range(const asw::Quad<float>& range) {
  std::vector<Tile*> ranged_map;

  for (auto& t : mapTiles) {
    if (t.getType() != 0 && t.getTransform().collides(range)) {
      ranged_map.push_back(&t);
    }
  }

  return ranged_map;
}

void TileMap::update(float deltaTime) {
  frame_timer += deltaTime;
}

// Draw at position
void TileMap::draw(const asw::Quad<float>& camera,
                   float destX,
                   float destY,
                   int layer) {
  if (layer == 1) {
    draw_layer(mapTilesBack, camera, destX, destY);

    // Draw semi-transparent buffer
    SDL_SetRenderDrawBlendMode(asw::display::renderer, SDL_BLENDMODE_BLEND);
    asw::draw::rectFill(asw::Quad<float>(0, 0, getWidth(), getHeight()),
                        asw::util::makeColor(0, 0, 0, 64));
    SDL_SetRenderDrawBlendMode(asw::display::renderer, SDL_BLENDMODE_NONE);
  }

  if (layer == 2) {
    draw_layer(mapTiles, camera, destX, destY);
  }
}

void TileMap::drawShadows(const asw::Quad<float>& camera,
                          float destX,
                          float destY) {
  // Draw shadow map
  for (unsigned int i = 0; i < shadowMap.size(); i++) {
    const auto kernelIdx = shadowMap[i];
    if (kernelIdx == 0) {
      continue;
    }

    auto position =
        asw::Vec2<float>((i % width) * 64.0F - camera.position.x - destX,
                         (i / width) * 64.0F - camera.position.y - destY);
    asw::draw::sprite(shadowTextures[kernelIdx], position);
  }
}

void TileMap::drawLights(const asw::Quad<float>& camera,
                         float destX,
                         float destY) {
  // Add lights
  if (!lighting) {
    return;
  }

  lightLayer.draw(camera, destX, destY);
}

// Draw a layer
void TileMap::draw_layer(std::vector<Tile>& t_map,
                         const asw::Quad<float>& camera,
                         float destX,
                         float destY) {
  int const frame = getFrame();

  for (auto& t : t_map) {
    if (t.getTransform().collides(camera)) {
      t.draw(camera.position.x - destX, camera.position.y - destY, frame);
    }
  }
}