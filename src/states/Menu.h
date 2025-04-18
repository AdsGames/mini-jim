#pragma once

#include "./State.h"

#include <asw/asw.h>
#include <string>
#include <vector>

#include "../Camera.h"
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
  asw::Vec2<float> scroll;
  asw::Vec2<float> scroll_dir;

  ProgramState next_state;

  // Lighting effects
  LightLayer lightLayer;

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

  Button buttons[7];
  Camera cam;
};
