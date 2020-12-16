#ifndef MENU_H
#define MENU_H

#include "State.h"

#include <allegro.h>
#include <loadpng.h>
#include <string>
#include <vector>

#include "TileMap.h"

#include "globals.h"
#include "utility/tools.h"

#include "Camera.h"
#include "ui/Button.h"

class Menu : public State {
 public:
  Menu();
  virtual ~Menu();
  virtual void update(StateEngine& engine) override;
  virtual void draw(BITMAP* buffer) override;

 private:
  // Disallow copy
  Menu(const Menu&);
  Menu& operator=(const Menu&);

  // Mouse hovering over button
  bool button_hover();

  // Change level (background)
  void change_level(int level);

  // Menu/GUI
  BITMAP *levelSelectNumber, *cursor, *menuselect, *menu, *help, *copyright,
      *credits;
  SAMPLE *click, *intro, *music;

  // Live background
  TileMap* tile_map;
  float scroll_x, scroll_y;
  int scroll_dir_x, scroll_dir_y;
  int next_state;

  // Lighting effects
  COLOR_MAP light_table;
  PALLETE pal;
  BITMAP *darkness, *darkness_old, *spotlight;

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
  Camera cam;
};

#endif  // MENU_H
