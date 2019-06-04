#include "TileTypeLoader.h"

#include <allegro.h>

#include <fstream>
#include <algorithm>

#include "utility/tools.h"
#include "globals.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma GCC diagnostic ignored "-Wswitch-default"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#pragma GCC diagnostic pop

std::vector<TileType*> TileTypeLoader::types;

TileType* TileTypeLoader::GetTile(int id) {
  for (const auto& t : types) {
    if (t -> GetID() == id) {
      return t;
    }
  }
  return nullptr;
}

int TileTypeLoader::GetID(std::string id_str) {
  for (const auto& t : types) {
    if (t -> GetIDStr() == id_str) {
      return t -> GetID();
    }
  }
  return -1;
}

int TileTypeLoader::GetCount() {
  return types.size();
}

void TileTypeLoader::LoadTypes(std::string path) {
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
  rapidxml::xml_node<> *allTiles = doc.first_node();

  // Define iterator
  rapidxml::xml_node<> *cTile = allTiles -> first_node("tile");

  // Parse data
  for (; cTile != nullptr; cTile = cTile -> next_sibling()) {

    std::string name = cTile -> first_attribute("name") -> value();

    std::string id_str = name;
    std::transform(id_str.begin(), id_str.end(), id_str.begin(), ::tolower);
    std::replace(id_str.begin(), id_str.end(), ' ', '_' );

    int id = atoi(cTile -> first_attribute("id") -> value());

    // Create tile
    TileType *tile = new TileType (id, name, id_str);

    // Images
    rapidxml::xml_node<> *img = cTile -> first_node("images") -> first_node("image");
    for (; img != nullptr; img = img -> next_sibling()) {
      BITMAP *image = load_png_ex ("images/blocks/" + std::string(img -> first_attribute("src") -> value()));
      tile -> AddImage(image);
      tile -> SetDimensions(0, 0, image -> w, image -> h);
    }

    // Add special feature
    if (cTile -> first_node("solid"))
      tile -> AddAttribute(solid);

    if (cTile -> first_node("light"))
      tile -> AddAttribute(light);

    if (cTile -> first_node("harmful"))
      tile -> AddAttribute(harmful);

    if (cTile -> first_node("slide"))
      tile -> AddAttribute(slide);

    // Add to types
    types.push_back (tile);
  }

  // Close
  file.close();
}
