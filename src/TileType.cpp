#include "TileType.h"

#include <algorithm>

TileType::TileType(short id, const std::string& name, const std::string& id_str)
    : id(id), x_1(0), x_2(0), y_1(0), y_2(0), name(name), id_str(id_str) {}

auto TileType::GetID() const -> short {
  return id;
}

std::string TileType::GetIDStr() const {
  return id_str;
}

std::string TileType::GetName() const {
  return name;
}

auto TileType::GetWidth() const -> int {
  return x_2 - x_1;
}

auto TileType::GetHeight() const -> int {
  return y_2 - y_1;
}

auto TileType::GetBBX() const -> int {
  return x_1;
}

auto TileType::GetBBY() const -> int {
  return y_1;
}

auto TileType::HasAttribute(int attribute) -> bool {
  return std::find(attributes.begin(), attributes.end(), attribute) !=
         attributes.end();
}

void TileType::AddImage(asw::Texture image) {
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
    asw::draw::sprite(images.at(frame % images.size()), x, y);
  }
}
