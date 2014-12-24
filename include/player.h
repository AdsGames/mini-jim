#ifndef PLAYER_H
#define PLAYER_H

#include <allegro.h>
#include <alpng.h>

#include "globals.h"
#include "tools.h"
#include "projectile.h"

#include "tileMap.h"

#define ANIMATION_SPEED 5

class player{
  public:
    player();
    ~player();

    void load_images( int newType);
    void load_sounds();
    void set_keys( int up, int down, int left, int right, int jump, int newJoyNumber);

    int getX();
    int getY();

    bool getFinished();

    bool getDead();

    vector<projectile> getBullets();

    void setFinished( bool newFinished);
    void setDead(bool newDead);
    void spawncommand(tileMap *newMap);

    void update(tileMap *newMap);
    void draw(BITMAP* temp, int tile_map_x, int tile_map_y);

  private:
    int x;
    int y;

    int checkpointPosition[2];

    float sprintSpeed;

    bool canFall;
    bool jumping;
    bool jumping_animation_done;
    bool dead;
    bool sprinting;

    bool finished;
    bool sliding;

    int walking_animation_sequence;
    int jumping_animation_sequence;
    int jump_height;
    int characterDir;
    int yVelocity;

    // Keys
    int upKey;
    int downKey;
    int leftKey;
    int rightKey;
    int jumpKey;
    int joyNumber;

    vector<projectile> bullets;

    // 0-3 left, 4-7 right, 8-11 up 12-17 jump left 18-23 jump slide 24-27
    BITMAP* player_images[28];

    //Sounds
    SAMPLE* walk1;
    SAMPLE* walk2;
    SAMPLE* jump;
    SAMPLE* die;
    SAMPLE* getItem;
    SAMPLE* getBonus;
    SAMPLE* win;
    SAMPLE* trapsnap;
    SAMPLE *chicken;
    SAMPLE *checkpoint;
};

#endif
