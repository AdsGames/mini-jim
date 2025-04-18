#pragma once

#include <string>
#include <vector>

#include <asw/asw.h>

class TileType {
 public:
  TileType(short id, const std::string& name, const std::string& id_str);

  void Draw(int x, int y, int frame);

  short GetID() const;
  std::string GetName() const;
  std::string GetIDStr() const;

  const asw::Quad<float>& GetBoundingBox() const;

  bool HasAttribute(int attribute);

  void AddImage(asw::Texture image);
  void AddAttribute(int attribute);
  void SetDimensions(const asw::Quad<float>& bounds);

 private:
  short id;
  std::string name;
  std::string id_str;
  asw::Quad<float> bounds;

  std::vector<int> attributes;
  std::vector<asw::Texture> images;
};
