#include "Player.h"

#include <climits>
#include "TileTypeLoader.h"

Player::Player(int number) {
  tm_animation.start();

  loadImages(number);
  loadSounds();

  if (number == 1) {
    setKeys(asw::input::Key::UP, asw::input::Key::DOWN, asw::input::Key::LEFT,
            asw::input::Key::RIGHT, asw::input::Key::RETURN, 0);
  } else {
    setKeys(asw::input::Key::W, asw::input::Key::S, asw::input::Key::A,
            asw::input::Key::D, asw::input::Key::SPACE, 1);
  }
}

// 0-3 left, 4-7 right, 8-11 up
void Player::loadImages(int type) {
  std::string prefix =
      "assets/images/character/character_" + std::to_string(type) + "_";

  tex_player[0] = asw::assets::loadTexture(prefix + "left_1.png");
  tex_player[1] = asw::assets::loadTexture(prefix + "left_2.png");
  tex_player[2] = asw::assets::loadTexture(prefix + "left_3.png");
  tex_player[3] = asw::assets::loadTexture(prefix + "left_4.png");

  tex_player[4] = asw::assets::loadTexture(prefix + "right_1.png");
  tex_player[5] = asw::assets::loadTexture(prefix + "right_2.png");
  tex_player[6] = asw::assets::loadTexture(prefix + "right_3.png");
  tex_player[7] = asw::assets::loadTexture(prefix + "right_4.png");

  tex_player[8] = asw::assets::loadTexture(prefix + "left_jump.png");
  tex_player[9] = asw::assets::loadTexture(prefix + "right_jump.png");

  tex_player[10] = asw::assets::loadTexture(prefix + "slide_left.png");
  tex_player[11] = asw::assets::loadTexture(prefix + "slide_right.png");

  tex_player[12] = asw::assets::loadTexture(prefix + "left_idle.png");
  tex_player[13] = asw::assets::loadTexture(prefix + "right_idle.png");
}

// Load sounds
void Player::loadSounds() {
  smp_chicken = asw::assets::loadSample("assets/sounds/chicken.wav");
  smp_walk[0] = asw::assets::loadSample("assets/sounds/walk_1.wav");
  smp_walk[1] = asw::assets::loadSample("assets/sounds/walk_2.wav");
  smp_jump = asw::assets::loadSample("assets/sounds/jump.wav");
  smp_die = asw::assets::loadSample("assets/sounds/die.wav");
  smp_win = asw::assets::loadSample("assets/sounds/win.wav");
  smp_trap_snap = asw::assets::loadSample("assets/sounds/trapsnap.wav");
  smp_checkpoint = asw::assets::loadSample("assets/sounds/checkpoint.wav");
}

// Set keys
void Player::setKeys(asw::input::Key up,
                     asw::input::Key down,
                     asw::input::Key left,
                     asw::input::Key right,
                     asw::input::Key jump,
                     int joy_number) {
  key_up = up;
  key_down = down;
  key_left = left;
  key_right = right;
  key_jump = jump;
  this->joy_number = joy_number;
}

// Set spawn
void Player::setSpawn(float x, float y) {
  last_checkpoint.first = x;
  last_checkpoint.second = y;
  transform.position.x = static_cast<float>(last_checkpoint.first);
  transform.position.y = static_cast<float>(last_checkpoint.second);
}

// Deathcount
auto Player::getDeathcount() const -> int {
  return death_count;
}

// Return X
auto Player::getX() const -> float {
  return transform.position.x;
}

// Return Y
auto Player::getY() const -> float {
  return transform.position.y;
}

// Get finished
auto Player::getFinished() const -> bool {
  return finished;
}

// Dead?
void Player::killSelf() {
  asw::sound::play(smp_die);
  player_state = CharacterState::Standing;
  death_count++;
  transform.position.x = static_cast<float>(last_checkpoint.first);
  transform.position.y = static_cast<float>(last_checkpoint.second);
  velocity = asw::Vec2<float>(0.0f, 0.0f);
}

// Movement
void Player::update(TileMap& fullMap, float dt) {
  // Get map around player
  const std::vector<Tile*> ranged_map =
      fullMap.get_tiles_in_range(transform.position.x - COLLISION_RANGE,
                                 transform.position.x + COLLISION_RANGE,
                                 transform.position.y - COLLISION_RANGE,
                                 transform.position.y + COLLISION_RANGE);

  // Gravity
  velocity.y += GRAVITY * dt;

  // Snap falling
  bool can_fall = true;
  const auto offset_transform = transform +
                                asw::Quad<float>(0, velocity.y * dt, 0, 0) +
                                asw::Quad<float>(8, 0, -16, 1);

  for (auto* t : ranged_map) {
    const auto& bb = t->getBoundingBox();
    if (t->containsAttribute(solid) && offset_transform.collides(bb) &&
        offset_transform.collidesTop(bb)) {
      can_fall = false;
      transform.position.y = bb.position.y - 64.0f;
      velocity.y = 0.0f;
      break;
    }
  }

  // Falling
  if (can_fall) {
    player_state = CharacterState::Jumping;
  }

  if (asw::input::isKeyDown(key_right)) {
    direction = CharacterDirection::Right;
  }

  if (asw::input::isKeyDown(key_left)) {
    direction = CharacterDirection::Left;
  }

  // State logic
  switch (player_state) {
    case CharacterState::Standing: {
      // Jump
      if (asw::input::wasKeyPressed(key_jump) ||
          asw::input::wasKeyPressed(key_up)) {
        velocity.y = JUMP_VELOCITY;
        asw::sound::play(smp_jump);
        player_state = CharacterState::Jumping;
      } else if (asw::input::isKeyDown(key_left) ||
                 asw::input::isKeyDown(key_right)) {
        player_state = CharacterState::Walking;
      } else {
        velocity.x = 0;
      }

      break;
    }

    case CharacterState::Walking: {
      if (asw::input::isKeyDown(key_down)) {
        player_state = CharacterState::Sliding;
      }

      if (!(asw::input::isKeyDown(key_left) ||
            asw::input::isKeyDown(key_right))) {
        player_state = CharacterState::Standing;
      }

      // Jump
      if (asw::input::wasKeyPressed(key_jump) ||
          asw::input::wasKeyPressed(key_up)) {
        velocity.y = JUMP_VELOCITY;
        asw::sound::play(smp_jump);
        player_state = CharacterState::Jumping;
        velocity.x *= JUMP_X_MULTIPLER;
      }

      if (direction == CharacterDirection::Right) {
        if (velocity.x < WALK_MIN_SPEED) {
          velocity.x = WALK_MIN_SPEED;
        } else if (velocity.x < WALK_MAX_SPEED) {
          velocity.x += WALK_ACCELERATION * dt;
        }
      } else if (direction == CharacterDirection::Left) {
        if (velocity.x > -WALK_MIN_SPEED) {
          velocity.x = -WALK_MIN_SPEED;
        } else if (velocity.x > -WALK_MAX_SPEED) {
          velocity.x -= WALK_ACCELERATION * dt;
        }
      }

      break;
    }

    case CharacterState::Jumping: {
      if (asw::input::isKeyDown(key_right) && velocity.x < WALK_MAX_SPEED) {
        velocity.x += WALK_ACCELERATION * dt;
      } else if (asw::input::isKeyDown(key_left) &&
                 velocity.x > -WALK_MAX_SPEED) {
        velocity.x -= WALK_ACCELERATION * dt;
      }

      if (!asw::input::isKeyDown(key_right) &&
          !asw::input::isKeyDown(key_left)) {
        velocity.x += (velocity.x > 0 ? -1 : 1) * JUMP_X_ACCELERATION * dt;
      }

      if (!can_fall) {
        player_state = CharacterState::Standing;
      }

      break;
    }

    case CharacterState::Sliding: {
      if (!asw::input::isKeyDown(key_down)) {
        player_state = CharacterState::Standing;
      }

      velocity.x += (velocity.x > 0 ? -1 : 1) * SLIDE_ACCELERATION * dt;
      if (std::abs(velocity.x) < 0.01f) {
        velocity.x = 0.0f;  // Stop completely if velocity is very small
      }

      // Jump
      if (asw::input::wasKeyPressed(key_jump) ||
          asw::input::wasKeyPressed(key_up)) {
        velocity.y = JUMP_VELOCITY;
        asw::sound::play(smp_jump);
        player_state = CharacterState::Jumping;
      }

      break;
    }

    default:
      break;
  }

  // Calculate new position
  const auto x_cmp = transform + asw::Quad<float>(velocity.x * dt, 0, 0, 0);
  const auto y_cmp = transform + asw::Quad<float>(0, velocity.y * dt, 0, 0);

  // Check for collision
  for (auto* t : ranged_map) {
    const auto& bb = t->getBoundingBox();

    // Left right
    if (x_cmp.collides(bb)) {
      if (t->containsAttribute(solid) ||
          (t->containsAttribute(slide) &&
           player_state != CharacterState::Sliding)) {
        if (velocity.x < 0.0f && x_cmp.collidesRight(bb)) {
          velocity.x = 0.0f;
        }

        if (velocity.x > 0.0f && x_cmp.collidesLeft(bb)) {
          velocity.x = 0.0f;
        }
      }
    }

    if (y_cmp.collides(bb)) {
      // Jumping
      if (t->containsAttribute(solid)) {
        if (y_cmp.collidesBottom(bb) && velocity.y < 0.0f) {
          velocity.y = 0.0f;
        }
      }

      // Harmful
      if (t->containsAttribute(harmful)) {
        if (t->getTypeStr() == "mouse_trap") {
          t->setType("mouse_trap_snapped");
          asw::sound::play(smp_trap_snap);
        } else if (t->getTypeStr() == "beak") {
          asw::sound::play(smp_chicken);
        }

        killSelf();
      }

      // Checkpoint
      if (t->getTypeStr() == "checkpoint") {
        if (last_checkpoint.first != t->getX() ||
            last_checkpoint.second != t->getY()) {
          last_checkpoint.first = t->getX();
          last_checkpoint.second = t->getY();
          asw::sound::play(smp_checkpoint, 50);
        }
      }

      // Finish
      if (t->getTypeStr() == "finish") {
        asw::sound::play(smp_win);
        finished = true;
      }
    }
  }

  // Apply velocity
  transform.position += velocity * dt;
}

// Draw character
void Player::draw(int tile_map_x, int tile_map_y) {
  const int ani_ticker =
      static_cast<int>(
          tm_animation.getElapsedTime<std::chrono::milliseconds>()) /
      100;

  // Tile map position and sprite offset
  auto position_offset = transform.position -
                         asw::Vec2<float>(tile_map_x, tile_map_y) -
                         asw::Vec2<float>(16.0f, 0);

  if (player_state == CharacterState::Jumping) {
    if (direction == CharacterDirection::Left) {
      asw::draw::sprite(tex_player[8], position_offset);
    } else {
      asw::draw::sprite(tex_player[9], position_offset);
    }
  } else if (player_state == CharacterState::Walking) {
    if (direction == CharacterDirection::Left) {
      asw::draw::sprite(tex_player[ani_ticker % 4], position_offset);
    } else {
      asw::draw::sprite(tex_player[4 + ani_ticker % 4], position_offset);
    }

  } else if (player_state == CharacterState::Standing) {
    if (direction == CharacterDirection::Left) {
      if (ani_ticker % 10 < 5) {
        asw::draw::sprite(tex_player[0], position_offset);
      } else {
        asw::draw::sprite(tex_player[12], position_offset);
      }
    } else {
      if (ani_ticker % 10 < 5) {
        asw::draw::sprite(tex_player[4], position_offset);
      } else {
        asw::draw::sprite(tex_player[13], position_offset);
      }
    }
  } else if (player_state == CharacterState::Sliding) {
    if (direction == CharacterDirection::Left) {
      asw::draw::sprite(tex_player[10], position_offset);
    } else {
      asw::draw::sprite(tex_player[11], position_offset);
    }
  }

  // Draw transform
  auto transform_offset = transform;
  transform_offset.position -= asw::Vec2<float>(tile_map_x, tile_map_y);

  asw::draw::rect(transform_offset, asw::util::makeColor(255, 0, 0));
}
