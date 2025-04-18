#include "TileType.h"

#include <algorithm>

TileType::TileType(short id, const std::string& name, const std::string& id_str)
    : id(id), name(name), id_str(id_str) {}

auto TileType::GetID() const -> short {
  return id;
}

std::string TileType::GetIDStr() const {
  return id_str;
}

std::string TileType::GetName() const {
  return name;
}

const asw::Quad<float>& TileType::GetBoundingBox() const {
  return bounds;
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

void TileType::SetDimensions(const asw::Quad<float>& bounds) {
  this->bounds = bounds;
}

void TileType::Draw(int x, int y, int frame) {
  if (!images.empty()) {
    asw::draw::sprite(images.at(frame % images.size()), asw::Vec2<float>(x, y));
  }
}
