#ifndef EDITOR_H
#define EDITOR_H

#include "GameState.h"

#include <allegro.h>
#include <string>
#include <vector>

#include "tileMap.h"
#include "tile.h"
#include "InputBox.h"
#include "Button.h"

class Editor : public GameState {
  public:
    Editor();
    ~Editor();

    virtual void update() override;
    virtual void draw() override;

  private:
    int layer;

    bool saving;
    bool opening;

    BITMAP *buffer;

    tileMap *tile_map;
    tile *pallette_tile;

    //Text input
    Button btn_save, btn_open;
    InputBox ib_save, ib_open;

    void save();
    void open();
    void edit();
};

#endif // EDITOR_H
