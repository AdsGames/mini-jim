#include "TileType.h"

#include <fstream>
#include <sstream>
#include <iostream>
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

TileType::TileType(int id, std::string name, std::string id_str) {
  this -> id = id;
  this -> name = name;
  this -> id_str = id_str;
}

TileType::~TileType() {
  //dtor
}

int TileType::GetWidth() {
  return this -> x_2;
}

int TileType::GetHeight() {
  return this -> y_2;
}

bool TileType::HasAttribute(int attribute) {
  return std::find (attributes.begin(), attributes.end(), attribute) != attributes.end();
}

void TileType::AddImage(BITMAP *image) {
  if (image)
    images.push_back(image);
}

void TileType::AddAttribute(int attribute) {
  attributes.push_back(attribute);
}

void TileType::SetDimensions(int x_1, int y_1, int x_2, int y_2) {
  this -> x_1 = x_1;
  this -> x_2 = x_2;
  this -> y_1 = y_1;
  this -> y_2 = y_2;
}

void TileType::Draw(BITMAP *buffer, int x, int y, int frame) {
  if (images.size() == 0) {
    textprintf_ex (buffer, font, x, y, makecol (0, 0, 0), -1, "Image");
    textprintf_ex (buffer, font, x, y + 20, makecol (0, 0, 0), -1, "%i NA", id);
    return;
  }

  draw_sprite (buffer, images.at(frame % images.size()), x, y);
}

int TileType::GetID() {
  return id;
}

std::string TileType::GetIDStr() {
  return id_str;
}


std::string TileType::GetName() {
  return name;
}




int TileTypeLoader::GetID(std::string id_str) {
  for (const auto& t : types) {
    if (t -> GetIDStr() == id_str) {
      return t -> GetID();
    }
  }
  return -1;
}

TileType* TileTypeLoader::GetTile(int id) {
  for (const auto& t : types) {
    if (t -> GetID() == id) {
      return t;
    }
  }
  std::cout << "Attempted to look up tile with id:" << id << " that does not exist" << std::endl;
  return nullptr;
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

    std::cout << "Loaded tile " << name << std::endl;

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
