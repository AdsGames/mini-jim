#ifndef EDITOR_H
#define EDITOR_H

#include "GameState.h"

#include <allegro.h>
#include <alpng.h>
#include <string>
#include <vector>

#include "tileMap.h"
#include "tile.h"
#include "globals.h"
#include "tools.h"

class Editor : public GameState
{
  private:
    int selectedTileType;
    int layer;

    bool saving;
    bool opening;

    BITMAP* buffer;

    tileMap *tile_map;
    tile *exampleTile;

    //Text input
    string  edittext;
    string::iterator iter;

    void init();
    void save();
    void open();
  protected:
  public:
    Editor();
    void update();
    void draw();
    ~Editor();
};

#endif // EDITOR_H
