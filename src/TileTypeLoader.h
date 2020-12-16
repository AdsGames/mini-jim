#ifndef TILETYPELOADER_H
#define TILETYPELOADER_H

#include <string>
#include <vector>

#include "TileType.h"

class TileTypeLoader {
 public:
  static void LoadTypes(const std::string& path);
  static TileType* GetTile(int id);
  static TileType* GetTile(const std::string& id_str);

 private:
  static std::vector<TileType*> types;
};

#endif  // TILETYPELOADER_H
