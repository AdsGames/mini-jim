#pragma once

#include <string>
#include <vector>

#include "TileType.h"

class TileTypeLoader {
 public:
  static void loadTypes(const std::string& path);
  static TileType* getTile(int id);
  static TileType* getTile(const std::string& id_str);

 private:
  static std::vector<TileType*> types;
};
