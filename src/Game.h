#ifndef GAME_H
#define GAME_H

#include "State.h"

#include <allegro.h>

#include "TileMap.h"
#include "Player.h"
#include "utility/Timer.h"
#include "Camera.h"

// Main game screen
class Game : public State {
  public:
    Game();
    virtual ~Game();
    virtual void update (StateEngine &engine) override;
    virtual void draw (BITMAP *buffer) override;
    void init();

  private:
    // Disallow copy
    Game (const Game &);
    Game &operator= (const Game &);

    BITMAP *screen1, *screen2;
    SAMPLE *countdown;
    SAMPLE *timeout;

    FONT *cooper;

    BITMAP *countdownImage;
    BITMAP *results;
    BITMAP *results_singleplayer;

    SAMPLE *mainMusic;

    COLOR_MAP light_table;
    PALLETE pal;
    BITMAP *darkness, *darkness_old, *spotlight;

    // Objects
    Player *player1, *player2;
    TileMap *tile_map;
    Timer tm_begin, tm_p1, tm_p2;
    Camera cam_1, cam_2;
};

#endif
