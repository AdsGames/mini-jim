#pragma once

#include "./State.h"

#include <asw/asw.h>
#include <asw/util/Timer.h>

#include "../Camera.h"
#include "../LightLayer.h"
#include "../Player.h"
#include "../TileMap.h"

// Main game screen
class Game : public asw::scene::Scene<ProgramState> {
 public:
  using asw::scene::Scene<ProgramState>::Scene;

  void init() override;
  void update(float dt) override;
  void draw() override;

 private:
  void setup();

  asw::Sample countdown;
  asw::Sample timeout;

  asw::Font cooper;

  asw::Texture countdownImage;
  asw::Texture results;
  asw::Texture results_singleplayer;

  asw::Music mainMusic;

  // Objects
  Player player1;
  Player player2;
  TileMap tile_map;
  Timer tm_begin, tm_p1, tm_p2;
  Camera cam_1, cam_2;
};
