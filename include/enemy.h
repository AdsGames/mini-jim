#ifndef ENEMY_H
#define ENEMY_H

#include <allegro.h>
#include <alpng.h>
#include <vector>

#include "globals.h"
#include "tools.h"

#include "tileMap.h"
#include "projectile.h"
#include "player.h"

#define ANIMATION_SPEED 5
#define JUMPING_HEIGHT 192

class enemy{
  public:
    enemy( int newX, int newY, int newType);
    ~enemy();

    void load_images();
    void load_sounds();

    int getX();
    int getY();

    bool getDead();

    void setDead(bool newDead);

    void update(tileMap *newMap, player newPlayer);
    void draw(BITMAP* temp, int tile_map_x, int tile_map_y);

  private:
    int x;
    int y;

    int width;
    int height;

    int speed;
    int type;

    //Cusomize!
    int jumpChance;
    int maxSteps;
    int minSteps;
    int jumpHeight;
    int walkChance;
    int jumpSpeed;

    bool canFall;
    bool jumping;
    bool dead;
    int walking_animation_sequence;
    int jumping_animation_sequence;
    int jump_height;
    int characterDir;
    int yVelocity;
    int walking_counter;

    vector<projectile> bullets;

    // 0-3 left, 4-7 right, 8 jump left 9 jump right 10 dead
    BITMAP* enemy_images[11];

    //Sounds
    SAMPLE* walk1;
    SAMPLE* walk2;
    SAMPLE* jump;
    SAMPLE* die;
    SAMPLE* getItem;
    SAMPLE* getBonus;
};

#endif
