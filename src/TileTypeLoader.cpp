#include "TileTypeLoader.h"

#include "./lib/aar/aar.h"

#include <algorithm>
#include <fstream>

#include "globals.h"
#include "utility/tools.h"

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

std::vector<TileType*> TileTypeLoader::types;

TileType* TileTypeLoader::GetTile(int id) {
  auto found = std::find_if(types.begin(), types.end(),
                            [&id](auto& t) { return t->GetID() == id; });

  return (found != types.end()) ? *found : nullptr;
}

TileType* TileTypeLoader::GetTile(const std::string& id_str) {
  auto found = std::find_if(types.begin(), types.end(), [&id_str](auto& t) {
    return t->GetIDStr() == id_str;
  });

  return (found != types.end()) ? *found : nullptr;
}

void TileTypeLoader::LoadTypes(const std::string& path) {
  // Open file or abort if it does not exist
  std::ifstream file(path);

  if (file.fail())
    return;

  // Create buffer
  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string content(buffer.str());

  // Get first node
  rapidxml::xml_document<> doc;
  doc.parse<0>(&content[0]);
  rapidxml::xml_node<>* allTiles = doc.first_node();

  // Define iterator
  rapidxml::xml_node<>* cTile = allTiles->first_node("tile");

  // Parse data
  for (; cTile != nullptr; cTile = cTile->next_sibling()) {
    std::string name = cTile->first_attribute("name")->value();

    std::string id_str = name;
    std::transform(id_str.begin(), id_str.end(), id_str.begin(), ::tolower);
    std::replace(id_str.begin(), id_str.end(), ' ', '_');

    int id = atoi(cTile->first_attribute("id")->value());

    // Create tile
    TileType* tile = new TileType(id, name, id_str);

    // Images
    if (cTile->first_node("images")) {
      rapidxml::xml_node<>* img =
          cTile->first_node("images")->first_node("image");

      for (; img != nullptr; img = img->next_sibling()) {
        aar::Texture* image = aar::load::bitmap(
            "assets/images/blocks/" +
            std::string(img->first_attribute("src")->value()));
        tile->AddImage(image);

        SDL_Point size = aar::util::getTextureSize(image);
        tile->SetDimensions(0, 0, size.x, size.y);
      }
    }

    // Bounding box
    rapidxml::xml_node<>* bounding_box = cTile->first_node("boundingBox");

    if (bounding_box) {
      int x_1 = atoi(bounding_box->first_attribute("x1")->value());
      int x_2 = atoi(bounding_box->first_attribute("x2")->value());
      int y_1 = atoi(bounding_box->first_attribute("y1")->value());
      int y_2 = atoi(bounding_box->first_attribute("y2")->value());
      tile->SetDimensions(x_1, y_1, x_2, y_2);
    }

    // Add special feature
    if (cTile->first_node("solid"))
      tile->AddAttribute(solid);

    if (cTile->first_node("light"))
      tile->AddAttribute(light);

    if (cTile->first_node("harmful"))
      tile->AddAttribute(harmful);

    if (cTile->first_node("slide"))
      tile->AddAttribute(slide);

    // Add to types
    types.push_back(tile);
  }

  // Close
  file.close();
}
