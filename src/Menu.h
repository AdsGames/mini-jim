#ifndef MENU_H
#define MENU_H

#include "State.h"

#include <asw/asw.h>
#include <string>
#include <vector>

#include "Camera.h"
#include "LightLayer.h"
#include "TileMap.h"
#include "globals.h"
#include "ui/Button.h"
#include "utility/tools.h"

class Menu : public State {
 public:
  explicit Menu(StateEngine& engine) : State(engine) {}

  void init() override;
  void update() override;
  void draw() override;
  void cleanup() override;

 private:
  // Mouse hovering over button
  auto button_hover() -> bool;

  // Change level (background)
  void change_level(int level);

  // Menu/GUI
  asw::Texture levelSelectNumber, cursor, menuselect, menu, help, copyright,
      credits;
  asw::Sample click, intro, music;

  // Live background
  TileMap* tile_map{};
  float scroll_x{};
  float scroll_y{};

  int scroll_dir_x{};
  int scroll_dir_y{};

  ProgramState next_state;

  // Lighting effects
  LightLayer lightLayer;

  asw::Font menuFont;

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
