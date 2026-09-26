#pragma once

#include <asw/asw.h>
#include <bitset>
#include <string>
#include <vector>

#include "globals.h"

class TileType {
 public:
  TileType(short id, const std::string& name, const std::string& id_str);

  void Draw(float x, float y, int frame);

  short GetID() const;
  std::string GetName() const;
  const std::string& GetIDStr() const;
  int GetLightLevel() const;

  const asw::Quadf& GetBoundingBox() const;

  bool HasAttribute(int attribute);

  void AddImage(asw::Texture image);
  void AddAttribute(int attribute);
  void SetDimensions(const asw::Quadf& bounds);
  void SetLightLevel(int level);

 private:
  short id;
  std::string name;
  std::string id_str;
  asw::Quadf bounds;
  int lightLevel{1};

  std::bitset<attribute_max> attributes;
  std::vector<asw::Texture> images;
};
