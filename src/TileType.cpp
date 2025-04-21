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

int TileType::GetLightLevel() const {
  return lightLevel;
}

std::string TileType::GetName() const {
  return name;
}

const asw::Quad<float>& TileType::GetBoundingBox() const {
  return bounds;
}

auto TileType::HasAttribute(int attribute) -> bool {
  if (attribute < 0 || attribute >= attribute_max) {
    return false;
  }

  return attributes.test(attribute);
}

void TileType::AddImage(asw::Texture image) {
  if (image) {
    images.push_back(image);
  }
}

void TileType::AddAttribute(int attribute) {
  if (attribute < 0 || attribute >= attribute_max) {
    return;
  }

  attributes.set(attribute);
}

void TileType::SetDimensions(const asw::Quad<float>& bounds) {
  this->bounds = bounds;
}

void TileType::SetLightLevel(int level) {
  lightLevel = level;
}

void TileType::Draw(float x, float y, int frame) {
  if (images.empty()) {
    return;
  }

  asw::draw::sprite(images.at(frame % images.size()), asw::Vec2<float>(x, y));
}
