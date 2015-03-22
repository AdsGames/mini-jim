#ifndef GAME_H
#define GAME_H

#include "GameState.h"

#include <allegro.h>
#include <alpng.h>
#include <string>
#include <vector>

#include "fmod/fmod.h"
#include "fmod/fmod_errors.h"

#include "globals.h"
#include "tools.h"

#include "tileMap.h"
#include "player.h"


using namespace std;

// Main game screen
class Game : public GameState
{
  private:
    BITMAP* buffer;
    BITMAP* screen1;
    BITMAP* screen2;

    SAMPLE *countdown;
    SAMPLE *timeout;

    FONT *cooper;

    BITMAP* countdownImage;
    BITMAP* results;
    BITMAP* results_singleplayer;
    BITMAP* spaceImage[4];

    FSOUND_STREAM* waitingMusic;
    FSOUND_STREAM* mainMusic;

    // Variables
    int animationFrame;
    static volatile int timer1;
    bool gameBegin;
    int totalTime[2];

    // Lighting effects
    COLOR_MAP light_table;
    PALLETE pal;
    bool lightingEnabled;
    BITMAP *darkness, *darkness_old, *lightBuffer, *spotlight;

    // Objects
    player player1;
    player player2;
    player *currentPlayer;

    bool player_1_ready;
    bool player_2_ready;

    bool deathFrame;

    tileMap *tile_map;
    tileMap *tile_map2;

    static void gameTicker();

  public:
    //Main loop functions
    Game();
    void init();
    void update();
    void draw();
    ~Game();
};

#endif
