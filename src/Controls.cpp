#include "Controls.h"

#include <asw/asw.h>

namespace {
using asw::input::ControllerAxis;
using asw::input::ControllerAxisBinding;
using asw::input::ControllerButton;
using asw::input::ControllerButtonBinding;
using asw::input::Key;
using asw::input::KeyBinding;

// Stick must pass this before it counts as a direction
constexpr float STICK_THRESHOLD = 0.5F;

void bind_direction(const std::string& name,
                    ControllerButton dpad,
                    ControllerAxis axis,
                    bool positive,
                    uint32_t index) {
  asw::input::bind_action(name, ControllerButtonBinding{dpad, index});
  asw::input::bind_action(
      name, ControllerAxisBinding{axis, index, STICK_THRESHOLD, positive});
}
}  // namespace

void controls::bind_ui() {
  constexpr auto any = asw::input::ANY_CONTROLLER;

  bind_direction(UI_UP, ControllerButton::DPadUp, ControllerAxis::LeftY, false,
                 any);
  bind_direction(UI_DOWN, ControllerButton::DPadDown, ControllerAxis::LeftY,
                 true, any);
  bind_direction(UI_LEFT, ControllerButton::DPadLeft, ControllerAxis::LeftX,
                 false, any);
  bind_direction(UI_RIGHT, ControllerButton::DPadRight, ControllerAxis::LeftX,
                 true, any);

  asw::input::bind_action(UI_CONFIRM, KeyBinding{Key::Return});
  asw::input::bind_action(UI_CONFIRM,
                          ControllerButtonBinding{ControllerButton::A, any});
  asw::input::bind_action(
      UI_CONFIRM, ControllerButtonBinding{ControllerButton::Start, any});

  asw::input::bind_action(UI_BACK, KeyBinding{Key::Escape});
  // Not B, a stray press would quit a level mid run
  asw::input::bind_action(UI_BACK,
                          ControllerButtonBinding{ControllerButton::Back, any});
}

controls::PlayerActions controls::bind_player(int number,
                                              uint32_t controller_index) {
  const std::string prefix = "p" + std::to_string(number) + "_";
  PlayerActions actions{prefix + "up", prefix + "down", prefix + "left",
                        prefix + "right", prefix + "jump"};

  for (const auto& name :
       {actions.up, actions.down, actions.left, actions.right, actions.jump}) {
    asw::input::unbind_action(name);
  }

  // Keyboard
  const bool first = number == 1;
  asw::input::bind_action(actions.up, KeyBinding{first ? Key::Up : Key::W});
  asw::input::bind_action(actions.down, KeyBinding{first ? Key::Down : Key::S});
  asw::input::bind_action(actions.left, KeyBinding{first ? Key::Left : Key::A});
  asw::input::bind_action(actions.right,
                          KeyBinding{first ? Key::Right : Key::D});
  asw::input::bind_action(actions.jump,
                          KeyBinding{first ? Key::Return : Key::Space});

  // Controller, up is d-pad only so the stick does not jump by accident
  asw::input::bind_action(
      actions.up,
      ControllerButtonBinding{ControllerButton::DPadUp, controller_index});
  bind_direction(actions.down, ControllerButton::DPadDown,
                 ControllerAxis::LeftY, true, controller_index);
  // B also slides
  asw::input::bind_action(
      actions.down,
      ControllerButtonBinding{ControllerButton::B, controller_index});
  bind_direction(actions.left, ControllerButton::DPadLeft,
                 ControllerAxis::LeftX, false, controller_index);
  bind_direction(actions.right, ControllerButton::DPadRight,
                 ControllerAxis::LeftX, true, controller_index);
  asw::input::bind_action(
      actions.jump,
      ControllerButtonBinding{ControllerButton::A, controller_index});

  return actions;
}

bool controls::any_controller_skip() {
  constexpr auto any = asw::input::ANY_CONTROLLER;
  return asw::input::get_controller_button_down(any, ControllerButton::A) ||
         asw::input::get_controller_button_down(any, ControllerButton::B) ||
         asw::input::get_controller_button_down(any, ControllerButton::Start);
}
