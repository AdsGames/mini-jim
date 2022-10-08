#ifndef MENU_H
#define MENU_H

#include "State.h"

#include <string>
#include <vector>
#include "./lib/aar/aar.h"

#include "TileMap.h"

#include "globals.h"
#include "utility/tools.h"

#include "Camera.h"
#include "ui/Button.h"

class Menu : public State {
 public:
  virtual void init() override;
  virtual void update(StateEngine& engine) override;
  virtual void draw() override;
  virtual void cleanup() override;

 private:
  // Mouse hovering over button
  bool button_hover();

  // Change level (background)
  void change_level(int level);

  // Menu/GUI
  aar::Texture *levelSelectNumber, *cursor, *menuselect, *menu, *help,
      *copyright, *credits;
  aar::Sample *click, *intro, *music;

  // Live background
  TileMap* tile_map;
  float scroll_x, scroll_y;
  int scroll_dir_x, scroll_dir_y;
  int next_state;

  // Lighting effects
  aar::Texture *darkness, *darkness_old, *spotlight;

  aar::Font* menuFont;

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
