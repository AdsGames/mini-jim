#include "utility/JoystickListener.h"

#include <allegro.h>

bool JoystickListener::button[MAX_JOYSTICK_BUTTONS] = {false};
bool JoystickListener::buttonPressed[MAX_JOYSTICK_BUTTONS] = {false};
bool JoystickListener::buttonReleased[MAX_JOYSTICK_BUTTONS] = {false};
bool JoystickListener::lastTicksButton[MAX_JOYSTICK_BUTTONS] = {false};

bool JoystickListener::anyButtonPressed = false;

// Check those joys!
void JoystickListener::update() {
  // Reset button pressed
  anyButtonPressed = false;

  // Check button just pressed
  for (int i = 0; i < MAX_JOYSTICK_BUTTONS; i++) {
    // Clear old values
    buttonPressed[i] = false;
    buttonReleased[i] = false;

    // Pressed since last tick?
    if (button[i] == true && lastTicksButton[i] == false) {
      buttonPressed[i] = true;
      anyButtonPressed = true;

    }

    // Released since last tick?
    if (button[i] == false && lastTicksButton[i] == true)
      buttonReleased[i] = true;

    // Button changed
    if (lastTicksButton[i] != button[i])
      lastTicksButton[i] = button[i];
  }

  // Get new values
  for (int i = 0; i < MAX_JOYSTICK_BUTTONS; i++) {
    if (lastTicksButton[i] != (bool)joy[0].button[i].b)
      lastTicksButton[i] = key[i];
  }
}

