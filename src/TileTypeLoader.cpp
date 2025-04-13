#include "TileTypeLoader.h"

#include <asw/asw.h>
#include <nlohmann/json.hpp>

#include <algorithm>
#include <fstream>
#include <sstream>

#include "globals.h"
#include "utility/tools.h"

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
    return;
  }

  // Create buffer
  nlohmann::json doc = nlohmann::json::parse(file);

  // Get first node
  for (auto const& cTile : doc) {
    std::string name = cTile["name"];
    short id = cTile["id"];

    std::string id_str = name;
    std::transform(id_str.begin(), id_str.end(), id_str.begin(), ::tolower);
    std::replace(id_str.begin(), id_str.end(), ' ', '_');

    // Create tile
    TileType* tile = new TileType(id, name, id_str);

    // Images
    if (cTile.contains("images")) {
      for (auto const& img : cTile["images"]) {
        asw::Texture image = asw::assets::loadTexture("assets/images/blocks/" +
                                                      img.get<std::string>());
        tile->AddImage(image);

        auto size = asw::util::getTextureSize(image);
        tile->SetDimensions(0, 0, size.x, size.y);
      }
    }

    // Bounding box
    if (cTile.contains("boundingBox")) {
      auto bounding_box = cTile["boundingBox"];
      int x_1 = bounding_box["x1"];
      int x_2 = bounding_box["x2"];
      int y_1 = bounding_box["y1"];
      int y_2 = bounding_box["y2"];
      tile->SetDimensions(x_1, y_1, x_2, y_2);
    }

    // Add special feature
    if (cTile.contains("solid")) {
      tile->AddAttribute(solid);
    }

    if (cTile.contains("light")) {
      tile->AddAttribute(light);
    }

    if (cTile.contains("harmful")) {
      tile->AddAttribute(harmful);
    }

    if (cTile.contains("slide")) {
      tile->AddAttribute(slide);
    }

    // Add to types
    types.push_back(tile);
  }

  // Close
  file.close();
}
