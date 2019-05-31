#ifndef PLAYER_H
#define PLAYER_H

#include <allegro.h>
#include <loadpng.h>

#include "globals.h"
#include "utility/tools.h"

#include "TileMap.h"
#include "Timer.h"

class player {
  public:
    player();
    ~player();

    void load_images (int player);
    void load_sounds();
    void set_keys (int up, int down, int left, int right, int jump, int joy_number);
    void set_spawn (int x, int y);

    int getX();
    int getY();
    int getDeathcount();

    bool getFinished();

    void update (TileMap *fullMap);
    void draw (BITMAP *temp, int tile_map_x, int tile_map_y);

  private:
    enum State {
      STATE_STANDING,
      STATE_JUMPING,
      STATE_SLIDING,
      STATE_WALKING,
    };

    void Die();

    int player_state;
    const float GRAVITY = 1.5f;
    bool canFall;
    int floorX;

    float x, y;

    int deathcount;

    int checkpointPosition[2];
    bool finished;

    int characterDir;
    float yVelocity, xVelocity;

    // Keys
    int upKey, downKey, leftKey, rightKey, jumpKey;
    int joyNumber;

    Timer tm_animation;

    // 0-3 left, 4-7 right, 8-11 up 12-17 jump left 18-23 jump slide 24-27 28-29 is idle
    BITMAP *player_images[14];

    //Sounds
    SAMPLE *walk[2];
    SAMPLE *jump;
    SAMPLE *die;
    SAMPLE *getItem;
    SAMPLE *getBonus;
    SAMPLE *win;
    SAMPLE *trapsnap;
    SAMPLE *chicken;
    SAMPLE *checkpoint;
};

#endif
