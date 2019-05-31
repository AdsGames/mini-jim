#ifndef GAME_H
#define GAME_H

#include "GameState.h"

#include <allegro.h>

#include "TileMap.h"
#include "player.h"
#include "utility/Timer.h"

// Main game screen
class Game : public GameState {
  public:
    Game();
    ~Game();

    void init();
    void update();
    void draw();

  private:
    BITMAP *buffer;
    BITMAP *screen1;
    BITMAP *screen2;
    SAMPLE *countdown;
    SAMPLE *timeout;

    FONT *cooper;

    BITMAP *countdownImage;
    BITMAP *results;
    BITMAP *results_singleplayer;
    BITMAP *spaceImage[4];

    SAMPLE *waitingMusic;
    SAMPLE *mainMusic;

    // Lighting effects
    COLOR_MAP light_table;
    PALLETE pal;
    bool lightingEnabled;
    BITMAP *darkness, *darkness_old, *lightBuffer, *spotlight;

    // Objects
    player player1, player2;
    TileMap *tile_map, *tile_map2;
    Timer tm_begin, tm_p1, tm_p2;
};

#endif
