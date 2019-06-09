#include "Player.h"

#include "utility/KeyListener.h"

#include "TileTypeLoader.h"

#define LEFT 0
#define RIGHT 4

player::player() {

  x = 128;
  y = 128;

  finished = false;

  characterDir = RIGHT;

  upKey = 0;
  downKey = 0;
  leftKey = 0;
  rightKey = 0;
  jumpKey = 0;

  yVelocity = 0;
  xVelocity = 0;

  deathcount = 0;

  checkpointPosition[0] = 0;
  checkpointPosition[1] = 0;

  player_state = STATE_STANDING;

  tm_animation.Start();
}

player::~player() {
  for (int i = 0; i < 14; i++)
    destroy_bitmap (player_images[i]);

  destroy_sample (walk[0]);
  destroy_sample (walk[1]);
  destroy_sample (jump);
  destroy_sample (die);
}

// 0-3 left, 4-7 right, 8-11 up
void player::load_images (int player) {
  std::string prefix = "images/character/character_" + std::to_string(player) + "_";

  player_images[0] = load_png_ex ((prefix + "left_1.png").c_str());
  player_images[1] = load_png_ex ((prefix + "left_2.png").c_str());
  player_images[2] = load_png_ex ((prefix + "left_3.png").c_str());
  player_images[3] = load_png_ex ((prefix + "left_4.png").c_str());

  player_images[4] = load_png_ex ((prefix + "right_1.png").c_str());
  player_images[5] = load_png_ex ((prefix + "right_2.png").c_str());
  player_images[6] = load_png_ex ((prefix + "right_3.png").c_str());
  player_images[7] = load_png_ex ((prefix + "right_4.png").c_str());

  player_images[8] = load_png_ex ((prefix + "left_jump.png").c_str());
  player_images[9] = load_png_ex ((prefix + "right_jump.png").c_str());

  player_images[10] = load_png_ex ((prefix + "slide_left.png").c_str());
  player_images[11] = load_png_ex ((prefix + "slide_right.png").c_str());

  player_images[12] = load_png_ex ((prefix + "left_idle.png").c_str());
  player_images[13] = load_png_ex ((prefix + "right_idle.png").c_str());
}

// Load sounds
void player::load_sounds() {
  chicken = load_sample_ex ("sounds/chicken.wav");
  walk[0] = load_sample_ex ("sounds/walk_1.wav");
  walk[1] = load_sample_ex ("sounds/walk_2.wav");
  jump = load_sample_ex ("sounds/jump.wav");
  die = load_sample_ex ("sounds/die.wav");
  win = load_sample_ex ("sounds/win.wav");
  trapsnap = load_sample_ex ("sounds/trapsnap.wav");
  checkpoint = load_sample_ex ("sounds/checkpoint.wav");
}

// Set keys
void player::set_keys (int up, int down, int left, int right, int jump, int joy_number) {
  upKey = up;
  downKey = down;
  leftKey = left;
  rightKey = right;
  jumpKey = jump;
  joyNumber = joy_number;
}

// Set spawn
void player::set_spawn (int x, int y) {
  checkpointPosition[0] = x;
  checkpointPosition[1] = y;
  this -> x = checkpointPosition[0];
  this -> y = checkpointPosition[1];
}

// Deathcount
int player::getDeathcount() {
  return deathcount;
}

// Return X
int player::getX() {
  return x;
}

// Return Y
int player::getY() {
  return y;
}

// Get finished
bool player::getFinished() {
  return finished;
}

// Dead?
void player::Die() {
  play_sample (die, 255, 125, 1000, 0);
  player_state = STATE_STANDING;
  deathcount++;
  x = checkpointPosition[0];
  y = checkpointPosition[1];
  xVelocity = 0;
  yVelocity = 0;
}

//Movement
void player::update (TileMap *fullMap) {
  //Collision stuff
  bool canMoveLeft = true;
  bool canMoveRight = true;
  bool canJumpUp = true;

  // Get map around player
  const int collision_range = 128;
  std::vector<tile *> ranged_map = fullMap -> get_tiles_in_range(x - collision_range, x + collision_range, y - collision_range, y + collision_range);

  //Check for collision
  for (auto t : ranged_map) {
    // Check moving LEFT
    if (collisionAny (x + 8 + xVelocity, x + 56 + xVelocity, t -> getX(), t -> getX() + t -> getWidth(),
                      y, y + 64, t -> getY(), t -> getY() + t -> getHeight())) {

      // Left right
      if (t -> containsAttribute (solid) || (t -> containsAttribute (slide) && player_state != STATE_SLIDING)) {
        if (collisionLeft (x + 8 + xVelocity, x + 56, t -> getX() + t -> getWidth()))
          canMoveLeft = false;

        if (collisionRight (x + 8, x + 56 + xVelocity, t -> getX()))
          canMoveRight = false;
      }
    }

    if (collisionAny (x + 16 + xVelocity, x + 48 + xVelocity, t -> getX(), t -> getX() + t -> getWidth(),
                      y + yVelocity, y + yVelocity + 64, t -> getY(), t -> getY() + t -> getHeight())) {
      // Jumping
      if (t -> containsAttribute (solid)) {
        if (collisionTop (t -> getY(), t -> getY() + t -> getHeight(), y + yVelocity))
          canJumpUp = false;
      }

      // Harmful
      if (t -> containsAttribute (harmful)) {
        if (t -> getTypeStr() == "mouse_trap") {
          t -> setType ("mouse_trap_snapped");
          play_sample (trapsnap, 255, 125, 1000, 0);
        }
        else if (t -> getTypeStr() == "beak") {
          play_sample (chicken, 255, 128, 1000, 0);
        }

        Die();
      }

      // Checkpoint
      if (t -> getTypeStr() == "checkpoint") {
        if (checkpointPosition[0] != t -> getX() || checkpointPosition[1] != t -> getY()) {
          checkpointPosition[0] = t -> getX();
          checkpointPosition[1] = t -> getY();
          play_sample (checkpoint, 255, 128, 1000, 0);
        }
      }

      // Finish
      if (t -> getTypeStr() == "finish") {
        play_sample (win, 255, 125, 1000, 0);
        finished = true;
      }
    }
  }

  // Falling
  if (canFall)
    player_state = STATE_JUMPING;

  if (key[rightKey])
    characterDir = RIGHT;

  if (key[leftKey])
    characterDir = LEFT;

  // State logic
  switch (player_state) {
    case STATE_STANDING: {
        //Jump
        if (KeyListener::keyPressed[jumpKey] || KeyListener::keyPressed[upKey]) {
          yVelocity = -24;
          play_sample (jump, 255, 125, 1000, 0);
          player_state = STATE_JUMPING;
        }
        else if (key[leftKey])
          player_state = STATE_WALKING;
        else if (key[rightKey])
          player_state = STATE_WALKING;
        else
          xVelocity = 0;

        break;
      }

    case STATE_WALKING: {
        if (key[downKey])
          player_state = STATE_SLIDING;

        if (!(key[leftKey] || key[rightKey]))
          player_state = STATE_STANDING;

        //Jump
        if (KeyListener::keyPressed[jumpKey] || KeyListener::keyPressed[upKey]) {
          yVelocity = -24;
          play_sample (jump, 255, 125, 1000, 0);
          player_state = STATE_JUMPING;
          xVelocity = xVelocity / 2;
        }

        // Animate
        if (int(tm_animation.GetElapsedTime<milliseconds>()) % 50 == 0) {
          play_sample (walk[random(0, 1)], 255, 125, 1000, 0);
        }

        if (characterDir == RIGHT) {
          if (xVelocity < 4)
            xVelocity = 4;
          else if (xVelocity < 12)
            xVelocity += 0.5;
        }
        else if (characterDir == LEFT) {
          if (xVelocity > -4)
            xVelocity = -4;
          else if (xVelocity > -12)
            xVelocity -= 0.5;
        }

        break;
      }

    case STATE_JUMPING: {
        if (key[rightKey] && xVelocity < 12)
          xVelocity += 0.5;
        else if (key[leftKey] && xVelocity > -12)
          xVelocity -= 0.5;

        if (!key[rightKey] && !key[leftKey])
          xVelocity *= 0.9;


        if (canFall) {
          if (!canJumpUp && yVelocity < 0.0f)
            yVelocity = 0;

          yVelocity += GRAVITY;
        }
        else {
          y = floorX - 64;
          yVelocity = 0;
          player_state = STATE_STANDING;
        }

        break;
      }

    case STATE_SLIDING: {
        if (!key[downKey])
          player_state = STATE_STANDING;

        (xVelocity < 0.01f && xVelocity > -0.01f) ? xVelocity = 0 : xVelocity *= 0.95f;

        //Jump
        if (KeyListener::keyPressed[jumpKey] || KeyListener::keyPressed[upKey]) {
          yVelocity = -24;
          play_sample (jump, 255, 125, 1000, 0);
          player_state = STATE_JUMPING;
        }

        break;
      }

    default:
      break;
  }

  if (!((canMoveRight && xVelocity > 0) || (canMoveLeft && xVelocity < 0)))
    xVelocity = 0;

  x += xVelocity;
  y += yVelocity;

  //Falling (calculated separately to ensure collision accurate)
  canFall = true;
  floorX = INT_MAX;

  for (auto t : ranged_map) {
    if (t -> containsAttribute (solid)) {
      if (collisionAny (x + 16, x + 48, t -> getX(), t -> getX() + t -> getWidth(),
                        y, y + 65 + yVelocity, t -> getY(), t -> getY() + t -> getHeight()) &&
          collisionTop (y, y + 65 + yVelocity, t -> getY())) {
        canFall = false;

        if (t -> getY() < floorX)
          floorX = t -> getY();
      }
    }
  }

  // Die
  if (x > fullMap -> getWidth() || x < 0 || y > fullMap -> getHeight())
    Die();
}

// Draw character
void player::draw (BITMAP *temp, int tile_map_x, int tile_map_y) {
  int ani_ticker = int(tm_animation.GetElapsedTime<milliseconds>()) / 100;

  if (player_state == STATE_JUMPING) {
    if (characterDir == LEFT)
      draw_sprite (temp, player_images[8], x - tile_map_x, y - tile_map_y);
    else
      draw_sprite (temp, player_images[9], x - tile_map_x, y - tile_map_y);
  }
  else if (player_state == STATE_WALKING) {
    draw_sprite (temp, player_images[characterDir + ani_ticker % 4], x - tile_map_x, y - tile_map_y);
  }
  else if (player_state == STATE_STANDING) {
    if (characterDir == LEFT) {
      if (ani_ticker % 10 < 5)
        draw_sprite (temp, player_images[0], x - tile_map_x, y - tile_map_y);
      else
        draw_sprite (temp, player_images[12], x - tile_map_x, y - tile_map_y);
    }
    else {
      if (ani_ticker % 10 < 5)
        draw_sprite (temp, player_images[4], x - tile_map_x, y - tile_map_y);
      else
        draw_sprite (temp, player_images[13], x - tile_map_x, y - tile_map_y);
    }
  }
  else if (player_state == STATE_SLIDING) {
    if (characterDir == LEFT)
      draw_sprite (temp, player_images[10], x - tile_map_x, y - tile_map_y);
    else
      draw_sprite (temp, player_images[11], x - tile_map_x, y - tile_map_y);
  }
}
