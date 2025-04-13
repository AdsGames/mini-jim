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

  int GetWidth() const;
  int GetHeight() const;

  int GetBBX() const;
  int GetBBY() const;

  bool HasAttribute(int attribute);

  void AddImage(asw::Texture image);
  void AddAttribute(int attribute);
  void SetDimensions(int x_1, int y_1, int x_2, int y_2);

 private:
  short id;
  std::string name{};
  std::string id_str{};
  int x_1, x_2, y_1, y_2;

  std::vector<int> attributes{};
  std::vector<asw::Texture> images{};
};
