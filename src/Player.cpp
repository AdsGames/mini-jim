#include "Player.h"

#include "TileTypeLoader.h"

#define LEFT 0
#define RIGHT 4

Player::Player(int number) {
  x = 128;
  y = 128;

  finished = false;
  canFall = false;
  floorX = 0;

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

  load_images(number);
  load_sounds();

  if (number == 1) {
    set_keys(SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT,
             SDL_SCANCODE_RIGHT, SDL_SCANCODE_RETURN, 0);
  } else {
    set_keys(SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, SDL_SCANCODE_D,
             SDL_SCANCODE_SPACE, 1);
  }
}

// 0-3 left, 4-7 right, 8-11 up
void Player::load_images(int type) {
  std::string prefix =
      "assets/images/character/character_" + std::to_string(type) + "_";

  player_images[0] = asw::assets::loadTexture(prefix + "left_1.png");
  player_images[1] = asw::assets::loadTexture(prefix + "left_2.png");
  player_images[2] = asw::assets::loadTexture(prefix + "left_3.png");
  player_images[3] = asw::assets::loadTexture(prefix + "left_4.png");

  player_images[4] = asw::assets::loadTexture(prefix + "right_1.png");
  player_images[5] = asw::assets::loadTexture(prefix + "right_2.png");
  player_images[6] = asw::assets::loadTexture(prefix + "right_3.png");
  player_images[7] = asw::assets::loadTexture(prefix + "right_4.png");

  player_images[8] = asw::assets::loadTexture(prefix + "left_jump.png");
  player_images[9] = asw::assets::loadTexture(prefix + "right_jump.png");

  player_images[10] = asw::assets::loadTexture(prefix + "slide_left.png");
  player_images[11] = asw::assets::loadTexture(prefix + "slide_right.png");

  player_images[12] = asw::assets::loadTexture(prefix + "left_idle.png");
  player_images[13] = asw::assets::loadTexture(prefix + "right_idle.png");
}

// Load sounds
void Player::load_sounds() {
  chicken = asw::assets::loadSample("assets/sounds/chicken.wav");
  walk[0] = asw::assets::loadSample("assets/sounds/walk_1.wav");
  walk[1] = asw::assets::loadSample("assets/sounds/walk_2.wav");
  jump = asw::assets::loadSample("assets/sounds/jump.wav");
  die = asw::assets::loadSample("assets/sounds/die.wav");
  win = asw::assets::loadSample("assets/sounds/win.wav");
  trapsnap = asw::assets::loadSample("assets/sounds/trapsnap.wav");
  checkpoint = asw::assets::loadSample("assets/sounds/checkpoint.wav");
}

// Set keys
void Player::set_keys(int up,
                      int down,
                      int left,
                      int right,
                      int jump,
                      int joy_number) {
  upKey = up;
  downKey = down;
  leftKey = left;
  rightKey = right;
  jumpKey = jump;
  this->joy_number = joy_number;
}

// Set spawn
void Player::set_spawn(int x, int y) {
  checkpointPosition[0] = x;
  checkpointPosition[1] = y;
  this->x = checkpointPosition[0];
  this->y = checkpointPosition[1];
}

// Deathcount
int Player::getDeathcount() const {
  return deathcount;
}

// Return X
int Player::getX() const {
  return x;
}

// Return Y
int Player::getY() const {
  return y;
}

// Get finished
bool Player::getFinished() const {
  return finished;
}

// Dead?
void Player::Die() {
  asw::sound::play(die);
  player_state = STATE_STANDING;
  deathcount++;
  x = checkpointPosition[0];
  y = checkpointPosition[1];
  xVelocity = 0;
  yVelocity = 0;
}

// Movement
void Player::update(TileMap* fullMap) {
  // Collision stuff
  bool canMoveLeft = true;
  bool canMoveRight = true;
  bool canJumpUp = true;

  // Get map around player
  const int collision_range = 128;
  std::vector<Tile*> ranged_map =
      fullMap->get_tiles_in_range(x - collision_range, x + collision_range,
                                  y - collision_range, y + collision_range);

  // Check for collision
  for (auto t : ranged_map) {
    // Check moving LEFT
    if (collisionAny(x + 8 + xVelocity, x + 56 + xVelocity, t->getX(),
                     t->getX() + t->getWidth(), y, y + 64, t->getY(),
                     t->getY() + t->getHeight())) {
      // Left right
      if (t->containsAttribute(solid) ||
          (t->containsAttribute(slide) && player_state != STATE_SLIDING)) {
        if (collisionLeft(x + 8 + xVelocity, x + 56, t->getX() + t->getWidth()))
          canMoveLeft = false;

        if (collisionRight(x + 8, x + 56 + xVelocity, t->getX()))
          canMoveRight = false;
      }
    }

    if (collisionAny(x + 16 + xVelocity, x + 48 + xVelocity, t->getX(),
                     t->getX() + t->getWidth(), y + yVelocity,
                     y + yVelocity + 64, t->getY(),
                     t->getY() + t->getHeight())) {
      // Jumping
      if (t->containsAttribute(solid)) {
        if (collisionTop(t->getY(), t->getY() + t->getHeight(), y + yVelocity))
          canJumpUp = false;
      }

      // Harmful
      if (t->containsAttribute(harmful)) {
        if (t->getTypeStr() == "mouse_trap") {
          t->setType("mouse_trap_snapped");
          asw::sound::play(trapsnap);
        } else if (t->getTypeStr() == "beak") {
          asw::sound::play(chicken);
        }

        Die();
      }

      // Checkpoint
      if (t->getTypeStr() == "checkpoint") {
        if (checkpointPosition[0] != t->getX() ||
            checkpointPosition[1] != t->getY()) {
          checkpointPosition[0] = t->getX();
          checkpointPosition[1] = t->getY();
          asw::sound::play(checkpoint, 50);
        }
      }

      // Finish
      if (t->getTypeStr() == "finish") {
        asw::sound::play(win);
        finished = true;
      }
    }
  }

  // Falling
  if (canFall)
    player_state = STATE_JUMPING;

  if (asw::input::keyboard.down[rightKey])
    characterDir = RIGHT;

  if (asw::input::keyboard.down[leftKey])
    characterDir = LEFT;

  // State logic
  switch (player_state) {
    case STATE_STANDING: {
      // Jump
      if (asw::input::keyboard.pressed[jumpKey] ||
          asw::input::keyboard.pressed[upKey]) {
        yVelocity = -24;
        asw::sound::play(jump);
        player_state = STATE_JUMPING;
      } else if (asw::input::keyboard.down[leftKey])
        player_state = STATE_WALKING;
      else if (asw::input::keyboard.down[rightKey])
        player_state = STATE_WALKING;
      else
        xVelocity = 0;

      break;
    }

    case STATE_WALKING: {
      if (asw::input::keyboard.down[downKey])
        player_state = STATE_SLIDING;

      if (!(asw::input::keyboard.down[leftKey] ||
            asw::input::keyboard.down[rightKey]))
        player_state = STATE_STANDING;

      // Jump
      if (asw::input::keyboard.pressed[jumpKey] ||
          asw::input::keyboard.pressed[upKey]) {
        yVelocity = -24;
        asw::sound::play(jump);
        player_state = STATE_JUMPING;
        xVelocity = xVelocity / 2;
      }

      // Animate
      if (int(tm_animation.GetElapsedTime<milliseconds>()) % 50 == 0) {
        asw::sound::play(walk[random(0, 1)]);
      }

      if (characterDir == RIGHT) {
        if (xVelocity < 4)
          xVelocity = 4;
        else if (xVelocity < 12)
          xVelocity += 0.5;
      } else if (characterDir == LEFT) {
        if (xVelocity > -4)
          xVelocity = -4;
        else if (xVelocity > -12)
          xVelocity -= 0.5;
      }

      break;
    }

    case STATE_JUMPING: {
      if (asw::input::keyboard.down[rightKey] && xVelocity < 12)
        xVelocity += 0.5;
      else if (asw::input::keyboard.down[leftKey] && xVelocity > -12)
        xVelocity -= 0.5;

      if (!asw::input::keyboard.down[rightKey] &&
          !asw::input::keyboard.down[leftKey])
        xVelocity *= 0.9;

      if (canFall) {
        if (!canJumpUp && yVelocity < 0.0f)
          yVelocity = 0;

        yVelocity += GRAVITY;
      } else {
        y = floorX - 64;
        yVelocity = 0;
        player_state = STATE_STANDING;
      }

      break;
    }

    case STATE_SLIDING: {
      if (!asw::input::keyboard.down[downKey])
        player_state = STATE_STANDING;

      (xVelocity < 0.01f && xVelocity > -0.01f) ? xVelocity = 0
                                                : xVelocity *= 0.95f;

      // Jump
      if (asw::input::keyboard.pressed[jumpKey] ||
          asw::input::keyboard.pressed[upKey]) {
        yVelocity = -24;
        asw::sound::play(jump);
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

  // Falling (calculated separately to ensure collision accurate)
  canFall = true;
  floorX = INT_MAX;

  for (auto t : ranged_map) {
    if (t->containsAttribute(solid)) {
      if (collisionAny(x + 16, x + 48, t->getX(), t->getX() + t->getWidth(), y,
                       y + 65 + yVelocity, t->getY(),
                       t->getY() + t->getHeight()) &&
          collisionTop(y, y + 65 + yVelocity, t->getY())) {
        canFall = false;

        if (t->getY() < floorX)
          floorX = t->getY();
      }
    }
  }

  // Die
  if (x > fullMap->getWidth() || x < 0 || y > fullMap->getHeight())
    Die();
}

// Draw character
void Player::draw(int tile_map_x, int tile_map_y) {
  int ani_ticker = int(tm_animation.GetElapsedTime<milliseconds>()) / 100;

  if (player_state == STATE_JUMPING) {
    if (characterDir == LEFT)
      asw::draw::sprite(player_images[8], x - tile_map_x, y - tile_map_y);
    else
      asw::draw::sprite(player_images[9], x - tile_map_x, y - tile_map_y);
  } else if (player_state == STATE_WALKING) {
    asw::draw::sprite(player_images[characterDir + ani_ticker % 4],
                      x - tile_map_x, y - tile_map_y);
  } else if (player_state == STATE_STANDING) {
    if (characterDir == LEFT) {
      if (ani_ticker % 10 < 5)
        asw::draw::sprite(player_images[0], x - tile_map_x, y - tile_map_y);
      else
        asw::draw::sprite(player_images[12], x - tile_map_x, y - tile_map_y);
    } else {
      if (ani_ticker % 10 < 5)
        asw::draw::sprite(player_images[4], x - tile_map_x, y - tile_map_y);
      else
        asw::draw::sprite(player_images[13], x - tile_map_x, y - tile_map_y);
    }
  } else if (player_state == STATE_SLIDING) {
    if (characterDir == LEFT)
      asw::draw::sprite(player_images[10], x - tile_map_x, y - tile_map_y);
    else
      asw::draw::sprite(player_images[11], x - tile_map_x, y - tile_map_y);
  }
}
