#pragma once

#include <cstdint>
#include <string>

// Input actions shared by keyboard and controllers
namespace controls {

// Menu and screen actions, any controller
inline constexpr const char* UI_UP = "ui_up";
inline constexpr const char* UI_DOWN = "ui_down";
inline constexpr const char* UI_LEFT = "ui_left";
inline constexpr const char* UI_RIGHT = "ui_right";
inline constexpr const char* UI_CONFIRM = "ui_confirm";
inline constexpr const char* UI_BACK = "ui_back";

// Action names for one player
struct PlayerActions {
  std::string up;
  std::string down;
  std::string left;
  std::string right;
  std::string jump;
};

// Bind menu and screen actions, call once after asw::core::init
void bind_ui();

// Bind a player's keys and controller, replacing earlier bindings.
// Player 1 uses the arrows and Return, player 2 uses WASD and Space.
PlayerActions bind_player(int number, uint32_t controller_index);

// Check if a controller button that skips a screen was pressed
bool any_controller_skip();

}  // namespace controls
