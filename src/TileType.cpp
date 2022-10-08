#include "TileType.h"

#include <algorithm>

TileType::TileType(short id, const std::string& name, const std::string& id_str)
    : id(id), name(name), id_str(id_str) {
  x_1 = 0;
  x_2 = 0;
  y_1 = 0;
  y_2 = 0;
}

short TileType::GetID() const {
  return id;
}

std::string TileType::GetIDStr() const {
  return id_str;
}

std::string TileType::GetName() const {
  return name;
}

int TileType::GetWidth() const {
  return x_2 - x_1;
}

int TileType::GetHeight() const {
  return y_2 - y_1;
}

int TileType::GetBBX() const {
  return x_1;
}

int TileType::GetBBY() const {
  return y_1;
}

bool TileType::HasAttribute(int attribute) {
  return std::find(attributes.begin(), attributes.end(), attribute) !=
         attributes.end();
}

void TileType::AddImage(aar::Texture* image) {
  if (image) {
    images.push_back(image);
  }
}

void TileType::AddAttribute(int attribute) {
  attributes.push_back(attribute);
}

void TileType::SetDimensions(int x_1, int y_1, int x_2, int y_2) {
  this->x_1 = x_1;
  this->x_2 = x_2;
  this->y_1 = y_1;
  this->y_2 = y_2;
}

void TileType::Draw(int x, int y, int frame) {
  if (images.size() > 0) {
    aar::draw::sprite(images.at(frame % images.size()), x, y);
  }
}
