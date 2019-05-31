#ifndef MENU_H
#define MENU_H

#include "GameState.h"

#include <allegro.h>
#include <loadpng.h>
#include <string>
#include <vector>

#include "TileMap.h"

#include "globals.h"
#include "utility/tools.h"

#include "Button.h"

class Menu : public GameState {
  public:
    Menu();
    virtual ~Menu();
    virtual void update() override;
    virtual void draw() override;

  private:
    // Mouse hovering over button
    bool button_hover();

    // Change level (background)
    void change_level(int level);

    // Menu/GUI
    BITMAP *buffer, *levelSelectNumber, *cursor, *menuselect, *menu, *help, *copyright, *credits;
    SAMPLE *click, *intro, *music;

    // Live background
    TileMap *tile_map;
    int scrollDirection;

    // Menu
    float selector_y, target_selector_y;
    int selected_button, move_to_button;

    enum button_names {
      BUTTON_START,
      BUTTON_START_MP,
      BUTTON_EDIT,
      BUTTON_HELP,
      BUTTON_EXIT,
      BUTTON_LEFT,
      BUTTON_RIGHT,
      NUM_BUTTONS
    };

    Button buttons[7];
};

#endif // MENU_H
