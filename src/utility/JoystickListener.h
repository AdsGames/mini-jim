/**
 * Joystick Listener
 * Allan Legemaate
 * Listens for joystick buttons JUST pressed or JUST released
 *   since the last tick
 * 23/01/2017
 **/

#ifndef JOYSTICKLISTENER_H
#define JOYSTICKLISTENER_H

#ifndef MAX_JOYSTICK_BUTTONS
#define MAX_JOYSTICK_BUTTONS 32
#endif

#include "../lib/aar/aar.h"

class JoystickListener {
 public:
  JoystickListener(){};
  virtual ~JoystickListener(){};

  static void update();

  static bool button[MAX_JOYSTICK_BUTTONS];
  static bool buttonPressed[MAX_JOYSTICK_BUTTONS];
  static bool buttonReleased[MAX_JOYSTICK_BUTTONS];
  static bool anyButtonPressed;

 private:
  static bool lastTicksButton[MAX_JOYSTICK_BUTTONS];
};

#endif  // JOYSTICKLISTENER_H
