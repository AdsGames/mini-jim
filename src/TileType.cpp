#include "TileType.h"

#include <algorithm>

TileType::TileType(short id, std::string name, std::string id_str) {
  this -> id = id;
  this -> name = name;
  this -> id_str = id_str;
}

TileType::~TileType() {
  //dtor
}

int TileType::GetWidth() {
  return x_2 - x_1;
}

int TileType::GetHeight() {
  return y_2 - y_1;
}

int TileType::GetBBX() {
  return x_1;
}

int TileType::GetBBY() {
  return y_1;
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
  if (images.size() > 0)
    draw_sprite (buffer, images.at(frame % images.size()), x, y);
}

short TileType::GetID() {
  return id;
}

std::string TileType::GetIDStr() {
  return id_str;
}


std::string TileType::GetName() {
  return name;
}
