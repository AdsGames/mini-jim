#pragma once

#include <array>
#include <utility>

#include <asw/asw.h>
#include <asw/util/Timer.h>

#include "globals.h"
#include "utility/tools.h"

#include "TileMap.h"

constexpr float COLLISION_RANGE = 128;

// Measured in pixels per ms
constexpr float JUMP_VELOCITY = -1.5F;
constexpr float JUMP_X_MULTIPLER = 0.8F;
constexpr float JUMP_X_ACCELERATION = 0.0035F;
constexpr float GRAVITY = 0.0058F;
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

  explicit Player(int number);

  void loadImages(int type);
  void loadSounds();
  void setKeys(int up, int down, int left, int right, int jump, int joy_number);
  void setSpawn(float x, float y);

  float getX() const;
  float getY() const;
  int getDeathcount() const;

  bool getFinished() const;

  void update(TileMap& fullMap, float dt);
  void draw(int tile_map_x, int tile_map_y);

 private:
  void killSelf();

  asw::Quad<float> transform{
      128.0F,
      128.0F,
      48.0F,
      64.0F,
  };

  // Pixels per MS
  asw::Vec2<float> velocity{0.0F, 0.0F};

  CharacterState player_state{CharacterState::Standing};
  CharacterDirection direction{CharacterDirection::Right};

  bool can_fall{false};
  int floor_x{0};
  int joy_number{0};

  int death_count{0};

  std::pair<int, int> last_checkpoint{0, 0};
  bool finished{false};

  // Keys
  int key_up{0};
  int key_down{0};
  int key_left{0};
  int key_right{0};
  int key_jump{0};

  Timer tm_animation{};

  // 0-3 left, 4-7 right, 8-11 up 12-17 jump left 18-23 jump slide 24-27 28-29
  // is idle
  std::array<asw::Texture, 14> tex_player{};

  // Sounds
  std::array<asw::Sample, 2> smp_walk{};
  asw::Sample smp_jump{};
  asw::Sample smp_die{};
  asw::Sample smp_win{};
  asw::Sample smp_trap_snap{};
  asw::Sample smp_chicken{};
  asw::Sample smp_checkpoint{};
};
