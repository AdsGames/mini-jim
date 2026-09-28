#pragma once

#include <array>
#include <utility>

#include <asw/asw.h>
#include <cstdint>
#include "Timer.h"

#include "Controls.h"

#include "globals.h"

#include "TileMap.h"

constexpr float COLLISION_RANGE = 256.0F;

// Measured in pixels per ms
constexpr float JUMP_VELOCITY = -1.5F;
constexpr float JUMP_X_MULTIPLER = 0.8F;
constexpr float JUMP_X_ACCELERATION = 0.0035F;
constexpr float GRAVITY = 0.0058F;
constexpr float TERMINAL_VELOCITY = 2.0F;
constexpr float WALK_MAX_SPEED = 0.8F;
constexpr float WALK_MIN_SPEED = 0.3F;
constexpr float WALK_ACCELERATION = 0.002F;
constexpr float SLIDE_ACCELERATION = 0.001F;

enum class CharacterDirection {
  Left,
  Right,
};

enum class CharacterState {
  Standing,
  Jumping,
  Sliding,
  Walking,
};

class Player {
 public:
  Player() = default;

  Player(int number, uint32_t controller_index);

  void loadImages(int type);
  void loadSounds();
  void setSpawn(const asw::Vec2f& position);

  int getDeathcount() const;

  const asw::Quadf& getTransform() const { return transform; }

  bool getFinished() const;

  // The camera is only used to pan this player's sounds
  void update(TileMap& fullMap, const asw::Camera& camera, float dt);
  void draw(const asw::Vec2f& offset);

 private:
  void killSelf();
  void playSound(const asw::Sample& sample, float volume = 1.0F) const;

  asw::Quadf transform{
      0.0F,
      0.0F,
      32.0F,
      64.0F,
  };

  // Pixels per MS
  asw::Vec2f velocity{0.0F, 0.0F};

  CharacterState player_state{CharacterState::Standing};
  CharacterDirection direction{CharacterDirection::Right};

  int death_count{0};

  asw::Vec2f last_checkpoint{0, 0};
  bool finished{false};

  // Keyboard and controller actions
  controls::PlayerActions actions{};

  // Stereo pan for sounds, from where the player is on screen
  float sound_pan{0.0F};

  Timer tm_animation{};

  // 0-3 left, 4-7 right, 8-11 up 12-17 jump left 18-23 jump slide 24-27 28-29
  // is idle
  std::array<asw::Texture, 7> tex_player{};

  // Sounds
  std::array<asw::Sample, 2> smp_walk{};
  asw::Sample smp_jump;
  asw::Sample smp_die;
  asw::Sample smp_win;
  asw::Sample smp_trap_snap;
  asw::Sample smp_chicken;
  asw::Sample smp_checkpoint;
};
