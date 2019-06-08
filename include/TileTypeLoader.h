#ifndef TILETYPELOADER_H
#define TILETYPELOADER_H

#include <vector>
#include <string>

#include "TileType.h"

class TileTypeLoader {
  public:
    static void LoadTypes(std::string path);
    static TileType *GetTile(int id);
    static TileType *GetTile(std::string id_str);
    static int GetCount();

  private:
    static std::vector<TileType*> types;
};

#endif // TILETYPELOADER_H
