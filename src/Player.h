#ifndef PLAYER_H
#define PLAYER_H

#include "./lib/aar/aar.h"

#include "globals.h"
#include "utility/tools.h"

#include "TileMap.h"
#include "utility/Timer.h"

class Player {
 public:
  explicit Player(int number);
  ~Player();

  void load_images(int type);
  void load_sounds();
  void set_keys(int up,
                int down,
                int left,
                int right,
                int jump,
                int joy_number);
  void set_spawn(int x, int y);

  int getX() const;
  int getY() const;
  int getDeathcount() const;

  bool getFinished() const;

  void update(TileMap* fullMap);
  void draw(int tile_map_x, int tile_map_y);

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
  int joy_number;

  float x, y;

  int deathcount;

  int checkpointPosition[2];
  bool finished;

  int characterDir;
  float yVelocity, xVelocity;

  // Keys
  int upKey, downKey, leftKey, rightKey, jumpKey;

  Timer tm_animation;

  // 0-3 left, 4-7 right, 8-11 up 12-17 jump left 18-23 jump slide 24-27 28-29
  // is idle
  aar::Texture* player_images[14];

  // Sounds
  aar::Sample* walk[2];
  aar::Sample* jump;
  aar::Sample* die;
  aar::Sample* win;
  aar::Sample* trapsnap;
  aar::Sample* chicken;
  aar::Sample* checkpoint;
};

#endif
