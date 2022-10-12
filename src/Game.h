#ifndef GAME_H
#define GAME_H

#include "State.h"

#include <asw/asw.h>
#include <asw/util/Timer.h>

#include "Camera.h"
#include "LightLayer.h"
#include "Player.h"
#include "TileMap.h"

// Main game screen
class Game : public State {
 public:
  explicit Game(StateEngine& engine) : State(engine) {}

  virtual void init() override;
  virtual void update() override;
  virtual void draw() override;
  virtual void cleanup() override;

  void setup();

 private:
  asw::Sample countdown;
  asw::Sample timeout;

  asw::Font cooper;

  asw::Texture countdownImage;
  asw::Texture results;
  asw::Texture results_singleplayer;

  asw::Sample mainMusic;

  LightLayer lightLayer;

  // Objects
  Player *player1, *player2;
  TileMap* tile_map;
  Timer tm_begin, tm_p1, tm_p2;
  Camera cam_1, cam_2;
};

#endif
