#pragma once

#include "./State.h"

#include <asw/asw.h>
#include <string>
#include <vector>

#include "../LightLayer.h"
#include "../TileMap.h"
#include "../globals.h"
#include "../ui/Button.h"

class Menu : public asw::scene::Scene<ProgramState> {
 public:
  using asw::scene::Scene<ProgramState>::Scene;

  void init() override;
  void update(float dt) override;
  void draw() override;

 private:
  // Change level (background)
  void change_level(int level);

  // Move focus with a controller
  void update_focus();

  // Menu/GUI
  asw::Texture levelSelectNumber;
  asw::Texture menuselect;
  asw::Texture menu;
  asw::Texture help;
  asw::Texture copyright;
  asw::Texture credits;

  asw::Sample click;
  asw::Sample intro;

  asw::Music music;

  // Live background
  TileMap tile_map;
  asw::Vec2f scroll;
  asw::Vec2f scroll_dir;

  ProgramState next_state;

  asw::Font menuFont;

  enum button_names {
    BUTTON_START,
    BUTTON_START_MP,
    BUTTON_HELP,
    BUTTON_EXIT,
    BUTTON_LEFT,
    BUTTON_RIGHT,
    NUM_BUTTONS
  };

  // Buttons in the menu list, in order from the top
  static constexpr int MENU_ITEMS = BUTTON_EXIT + 1;

  Button buttons[7];
  asw::Camera cam;

  // Focused button when using a controller, one of the first MENU_ITEMS
  int focus{BUTTON_START};
};
