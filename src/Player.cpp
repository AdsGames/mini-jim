#include "Player.h"

#include <algorithm>
#include <climits>
#include "TileTypeLoader.h"

namespace {
// Largest stereo pan for player sounds, full pan is harsh on headphones
constexpr float MAX_SOUND_PAN = 0.6F;

// Random pitch change per play so repeated sounds are not robotic
constexpr float SOUND_PITCH_VARIATION = 0.06F;
}  // namespace

Player::Player(int number, uint32_t controller_index) {
  tm_animation.start();

  loadImages(number);
  loadSounds();

  actions = controls::bind_player(number, controller_index);
}

// 0-3 left, 4-7 right, 8-11 up
void Player::loadImages(int type) {
  std::string prefix =
      "assets/images/character/character_" + std::to_string(type) + "_";

  tex_player[0] = asw::assets::load_texture(prefix + "right_1.png");
  tex_player[1] = asw::assets::load_texture(prefix + "right_2.png");
  tex_player[2] = asw::assets::load_texture(prefix + "right_3.png");
  tex_player[3] = asw::assets::load_texture(prefix + "right_4.png");
  tex_player[4] = asw::assets::load_texture(prefix + "right_jump.png");
  tex_player[5] = asw::assets::load_texture(prefix + "slide_right.png");
  tex_player[6] = asw::assets::load_texture(prefix + "right_idle.png");
}

// Load sounds
void Player::loadSounds() {
  smp_chicken = asw::assets::load_sample("assets/sounds/chicken.wav");
  smp_walk[0] = asw::assets::load_sample("assets/sounds/walk_1.wav");
  smp_walk[1] = asw::assets::load_sample("assets/sounds/walk_2.wav");
  smp_jump = asw::assets::load_sample("assets/sounds/jump.wav");
  smp_die = asw::assets::load_sample("assets/sounds/die.wav");
  smp_win = asw::assets::load_sample("assets/sounds/win.wav");
  smp_trap_snap = asw::assets::load_sample("assets/sounds/trapsnap.wav");
  smp_checkpoint = asw::assets::load_sample("assets/sounds/checkpoint.wav");
}

// Play a sound panned to this player's side of the screen
void Player::playSound(const asw::Sample& sample, float volume) const {
  asw::sound::PlayOptions options;
  options.volume = volume;
  options.pan = sound_pan;
  options.pitch_variation = SOUND_PITCH_VARIATION;
  asw::sound::play(sample, options);
}

// Set spawn
void Player::setSpawn(const asw::Vec2f& position) {
  last_checkpoint = position;
  transform.position = position;
}

// Deathcount
auto Player::getDeathcount() const -> int {
  return death_count;
}

// Get finished
auto Player::getFinished() const -> bool {
  return finished;
}

// Dead?
void Player::killSelf() {
  playSound(smp_die);
  asw::sound::duck(asw::sound::Bus::Music, 0.4F, 0.4F);
  player_state = CharacterState::Standing;
  death_count++;
  transform.position = last_checkpoint;
  velocity = asw::Vec2f(0.0f, 0.0f);
}

// Movement
void Player::update(TileMap& fullMap, const asw::Camera& camera, float dt) {
  const auto view_width = camera.get_view().size.x;
  if (view_width > 0.0F) {
    const float screen_x = camera.world_to_screen(transform.get_center()).x;
    sound_pan =
        std::clamp(((screen_x / view_width) * 2.0F) - 1.0F, -1.0F, 1.0F) *
        MAX_SOUND_PAN;
  }

  // Get map around player
  const std::vector<Tile*> ranged_map = fullMap.get_tiles_in_range(
      transform + asw::Quadf(-256.0F, -256.0F, 512.0F, 512.0F));

  // Gravity
  velocity.y += GRAVITY * dt;
  if (velocity.y > TERMINAL_VELOCITY) {
    velocity.y = TERMINAL_VELOCITY;
  }

  // Snap falling
  bool can_fall = true;
  const auto offset_transform = transform +
                                asw::Quadf(0, velocity.y * dt, 0, 0) +
                                asw::Quadf(8, 0, -16, 1);

  for (auto* t : ranged_map) {
    const auto& bb = t->getTransform();
    if (t->containsAttribute(solid) && offset_transform.collides(bb) &&
        offset_transform.collides_top(bb)) {
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

  if (asw::input::get_action(actions.right)) {
    direction = CharacterDirection::Right;
  }

  if (asw::input::get_action(actions.left)) {
    direction = CharacterDirection::Left;
  }

  // State logic
  switch (player_state) {
    case CharacterState::Standing: {
      // Jump
      if (asw::input::get_action_down(actions.jump) ||
          asw::input::get_action_down(actions.up)) {
        velocity.y = JUMP_VELOCITY;
        playSound(smp_jump);
        player_state = CharacterState::Jumping;
      } else if (asw::input::get_action(actions.left) ||
                 asw::input::get_action(actions.right)) {
        player_state = CharacterState::Walking;
      } else {
        velocity.x = 0;
      }

      break;
    }

    case CharacterState::Walking: {
      if (asw::input::get_action(actions.down)) {
        player_state = CharacterState::Sliding;
      }

      if (!(asw::input::get_action(actions.left) ||
            asw::input::get_action(actions.right))) {
        player_state = CharacterState::Standing;
      }

      // Jump
      if (asw::input::get_action_down(actions.jump) ||
          asw::input::get_action_down(actions.up)) {
        velocity.y = JUMP_VELOCITY;
        playSound(smp_jump);
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
      if (asw::input::get_action(actions.right) &&
          velocity.x < WALK_MAX_SPEED) {
        velocity.x += WALK_ACCELERATION * dt;
      } else if (asw::input::get_action(actions.left) &&
                 velocity.x > -WALK_MAX_SPEED) {
        velocity.x -= WALK_ACCELERATION * dt;
      }

      if (!asw::input::get_action(actions.right) &&
          !asw::input::get_action(actions.left)) {
        velocity.x += (velocity.x > 0 ? -1 : 1) * JUMP_X_ACCELERATION * dt;
      }

      if (!can_fall) {
        player_state = CharacterState::Standing;
      }

      break;
    }

    case CharacterState::Sliding: {
      if (!asw::input::get_action(actions.down)) {
        player_state = CharacterState::Standing;
      }

      velocity.x += (velocity.x > 0 ? -1 : 1) * SLIDE_ACCELERATION * dt;
      if (std::abs(velocity.x) < 0.01f) {
        velocity.x = 0.0f;  // Stop completely if velocity is very small
      }

      // Jump
      if (asw::input::get_action_down(actions.jump) ||
          asw::input::get_action_down(actions.up)) {
        velocity.y = JUMP_VELOCITY;
        playSound(smp_jump);
        player_state = CharacterState::Jumping;
      }

      break;
    }

    default:
      break;
  }

  // Calculate new position
  const auto x_cmp = transform + asw::Quadf(velocity.x * dt, 0, 0, 0);
  const auto y_cmp = transform + asw::Quadf(0, velocity.y * dt, 0, 0);

  // Check for collision
  for (auto* t : ranged_map) {
    const auto& bb = t->getTransform();
    const auto* t_type = t->getType();

    // Left right
    if (x_cmp.collides(bb)) {
      if (t->containsAttribute(solid) ||
          (t->containsAttribute(slide) &&
           player_state != CharacterState::Sliding)) {
        if (velocity.x < 0.0f && x_cmp.collides_right(bb)) {
          velocity.x = 0.0f;
        }

        if (velocity.x > 0.0f && x_cmp.collides_left(bb)) {
          velocity.x = 0.0f;
        }
      }
    }

    if (y_cmp.collides(bb)) {
      // Jumping
      if (t->containsAttribute(solid)) {
        if (y_cmp.collides_bottom(bb) && velocity.y < 0.0f) {
          velocity.y = 0.0f;
        }
      }

      // Harmful
      if (t->containsAttribute(harmful)) {
        if (t_type->GetIDStr() == "mouse_trap") {
          t->setType("mouse_trap_snapped");
          playSound(smp_trap_snap);
        } else if (t_type->GetIDStr() == "beak") {
          playSound(smp_chicken);
        }

        // Respawned at checkpoint, skip remaining collisions this tick
        killSelf();
        return;
      }

      // Checkpoint
      if (t_type->GetIDStr() == "checkpoint") {
        if (last_checkpoint.x != bb.position.x ||
            last_checkpoint.y != bb.position.y) {
          last_checkpoint = bb.position;
          playSound(smp_checkpoint, 50.0F / 255.0F);
        }
      }

      // Finish
      if (t_type->GetIDStr() == "finish") {
        if (!finished) {
          playSound(smp_win);
          asw::sound::duck(asw::sound::Bus::Music, 0.3F, 1.5F, 0.3F);
        }
        finished = true;
      }
    }
  }

  // Apply velocity
  transform.position += velocity * dt;

  // Push back out of any solid tile the move still ended inside
  for (auto* t : ranged_map) {
    if (!t->containsAttribute(solid)) {
      continue;
    }

    const auto push = transform.get_push_out(t->getTransform());
    transform.position += push;

    if (push.x != 0.0F) {
      velocity.x = 0.0F;
    }

    if (push.y != 0.0F) {
      velocity.y = 0.0F;
    }
  }

  // Die
  if (transform.position.x > fullMap.getWidth() ||
      transform.position.x < 0.0f ||
      transform.position.y > fullMap.getHeight()) {
    killSelf();
  }
}

// Draw character
void Player::draw(const asw::Vec2f& offset) {
  const int ani_ticker =
      static_cast<int>(
          tm_animation.getElapsedTime<std::chrono::milliseconds>()) /
      100;

  // Tile map position and sprite offset
  auto position_offset = transform.position - offset - asw::Vec2f(16.0f, 0);

  if (player_state == CharacterState::Jumping) {
    if (direction == CharacterDirection::Right) {
      asw::draw::sprite(tex_player[4], position_offset);
    } else {
      asw::draw::sprite_flip(tex_player[4], position_offset, true, false);
    }
  } else if (player_state == CharacterState::Walking) {
    if (direction == CharacterDirection::Right) {
      asw::draw::sprite(tex_player[ani_ticker % 4], position_offset);
    } else {
      asw::draw::sprite_flip(tex_player[ani_ticker % 4], position_offset, true,
                             false);
    }

  } else if (player_state == CharacterState::Standing) {
    if (direction == CharacterDirection::Right) {
      asw::draw::sprite(tex_player[6], position_offset);
    } else {
      asw::draw::sprite_flip(tex_player[6], position_offset, true, false);
    }
  } else if (player_state == CharacterState::Sliding) {
    if (direction == CharacterDirection::Right) {
      asw::draw::sprite(tex_player[5], position_offset);
    } else {
      asw::draw::sprite_flip(tex_player[5], position_offset, true, false);
    }
  }
}
