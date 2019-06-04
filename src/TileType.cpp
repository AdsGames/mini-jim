#include "TileType.h"

#include <algorithm>

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
