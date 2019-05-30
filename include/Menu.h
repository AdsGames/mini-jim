#ifndef MENU_H
#define MENU_H

#include "GameState.h"

#include <allegro.h>
#include <loadpng.h>
#include <string>
#include <vector>

#include "tilemap.h"

#include "globals.h"
#include "tools.h"

#include "Button.h"

class Menu : public GameState {
  public:
    Menu();
    virtual ~Menu();
    virtual void update() override;
    virtual void draw() override;

  private:
    // Menu/GUI
    BITMAP *buffer, *levelSelectNumber, *cursor, *menuselect, *menu, *help, *copyright, *credits;
    SAMPLE *click, *intro, *music;

    // Live background
    tileMap *tile_map;
    int scrollDirection;

    // Menu
    float selector_y, target_selector_y;
    int selected_button, move_to_button;

    Button btn_start, btn_edit, btn_help, btn_exit, btn_start_mp, btn_left, btn_right;
};

#endif // MENU_H
