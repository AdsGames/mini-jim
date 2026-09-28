#pragma once

#include "./State.h"

#include <asw/asw.h>
#include <string>
#include <vector>

#include "../LightLayer.h"
#include "../TileMap.h"
#include "../globals.h"

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
  asw::Vec2f scroll;
  asw::Vec2f scroll_dir;

  ProgramState next_state;

  asw::Font menuFont;

  asw::Camera cam;

  // Menu buttons, focus shows as the hover image
  asw::ui::Root ui;
  asw::ui::Button* help_button{nullptr};
};
