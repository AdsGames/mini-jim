#include "TileTypeLoader.h"

#include <asw/asw.h>
#include <algorithm>
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>

#include "globals.h"

std::vector<TileType*> TileTypeLoader::types;

auto TileTypeLoader::getTile(int id) -> TileType* {
  auto found = std::find_if(types.begin(), types.end(),
                            [&id](auto& t) { return t->GetID() == id; });

  return (found != types.end()) ? *found : nullptr;
}

auto TileTypeLoader::getTile(const std::string& id_str) -> TileType* {
  auto found = std::find_if(types.begin(), types.end(), [&id_str](auto& t) {
    return t->GetIDStr() == id_str;
  });

  return (found != types.end()) ? *found : nullptr;
}

void TileTypeLoader::loadTypes(const std::string& path) {
  // Open file or abort if it does not exist
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file " << path << '\n';
    return;
  }

  // Create buffer
  nlohmann::json doc = nlohmann::json::parse(file);

  // Create images map
  // Must be done first to get animation images
  std::cout << "Loading images..." << '\n';
  std::map<short, asw::Texture> images;
  for (auto const& cTile : doc["tiles"]) {
    const short id = cTile["id"];
    const std::string image = cTile["image"];
    images[id] = asw::assets::loadTexture("assets/images/" + image);
  }

  // Get first node
  std::cout << "Loading tiles..." << '\n';
  for (auto const& cTile : doc["tiles"]) {
    // Numeric identifier
    const short id = cTile["id"];

    // Get properties
    std::string name;
    bool isSolid = false;
    bool isHarmful = false;
    bool isSlide = false;
    bool isShadow = false;
    int lightLevel = 0;

    // Invalid tile
    if (!cTile.contains("properties")) {
      continue;
    }

    for (auto const& property : cTile["properties"]) {
      if (property["name"] == "name") {
        name = property["value"];
      } else if (property["name"] == "solid") {
        isSolid = property["value"];
      } else if (property["name"] == "harmful") {
        isHarmful = property["value"];
      } else if (property["name"] == "slide") {
        isSlide = property["value"];
      } else if (property["name"] == "shadow") {
        isShadow = property["value"];
      } else if (property["name"] == "light_level") {
        lightLevel = property["value"];
      }
    }

    // Parse name
    std::string id_str = name;
    std::transform(id_str.begin(), id_str.end(), id_str.begin(), ::tolower);
    std::replace(id_str.begin(), id_str.end(), ' ', '_');

    // Create tile
    TileType* tile = new TileType(id, name, id_str);

    // Images
    std::vector<short> image_ids;
    if (cTile.contains("animation")) {
      for (auto const& ani : cTile["animation"]) {
        image_ids.push_back(ani["tileid"]);
      }
    } else {
      image_ids.push_back(id);
    }

    // Add images
    for (auto const& image_id : image_ids) {
      auto it = images.find(image_id);
      if (it != images.end()) {
        tile->AddImage(it->second);
        auto size = asw::util::getTextureSize(it->second);
        tile->SetDimensions(asw::Quad<float>(0, 0, size.x, size.y));
      }
    }

    // Bounding box
    if (cTile.contains("objectgroup")) {
      if (!cTile["objectgroup"].contains("objects") ||
          cTile["objectgroup"]["objects"].empty()) {
        std::cerr << "Error: No objects found in tile " << name << '\n';
        continue;
      }

      const auto bounding_box = cTile["objectgroup"]["objects"][0];
      const int x = bounding_box["x"];
      const int width = bounding_box["width"];
      const int y = bounding_box["y"];
      const int height = bounding_box["height"];

      tile->SetDimensions(asw::Quad<float>(x, y, width, height));
    }

    // Add special feature
    if (isSolid) {
      tile->AddAttribute(solid);
    }

    if (lightLevel > 0.0F) {
      tile->AddAttribute(light);
      tile->SetLightLevel(lightLevel);
    }

    if (isHarmful) {
      tile->AddAttribute(harmful);
    }

    if (isSlide) {
      tile->AddAttribute(slide);
    }

    if (isShadow) {
      tile->AddAttribute(shadow);
    }

    // Add to types
    types.push_back(tile);
  }

  // Close
  file.close();
}
