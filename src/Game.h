#ifndef GAME_H
#define GAME_H

#include "State.h"

#include "./lib/aar/aar.h"

#include "Camera.h"
#include "Player.h"
#include "TileMap.h"
#include "utility/Timer.h"

// Main game screen
class Game : public State {
 public:
  virtual ~Game();

  virtual void init(aar::Window* window);
  virtual void update(StateEngine& engine) override;
  virtual void draw(aar::Renderer* buffer) override;
  void setup();

 private:
  aar::Sample* countdown;
  aar::Sample* timeout;

  aar::Font* cooper;

  aar::Texture* countdownImage;
  aar::Texture* results;
  aar::Texture* results_singleplayer;

  aar::Sample* mainMusic;

  // COLOR_MAP light_table;
  // PALLETE pal;
  aar::Texture *darkness, *darkness_old, *spotlight;

  // Objects
  Player *player1, *player2;
  TileMap* tile_map;
  Timer tm_begin, tm_p1, tm_p2;
  Camera cam_1, cam_2;
};

#endif
