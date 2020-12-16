#include "KeyListener.h"

#include <allegro.h>

bool KeyListener::keyPressed[KL_KEY_MAX] = { false};
bool KeyListener::keyReleased[KL_KEY_MAX] = { false};
bool KeyListener::lastTicksKey[KL_KEY_MAX] = { false};

// Check those keys!
void KeyListener::update() {
  // Check key just pressed
  for (int i = 0; i < KL_KEY_MAX; i++) {
    // Clear old values
    keyPressed[i] = false;
    keyReleased[i] = false;

    // Pressed since last tick?
    if ((bool)key[i] == true && lastTicksKey[i] == false)
      keyPressed[i] = true;

    // Released since last tick?
    if ((bool)key[i] == false && lastTicksKey[i] == true)
      keyReleased[i] = true;
  }


  // Get new values
  for (int i = 0; i < KL_KEY_MAX; i++) {
    if (lastTicksKey[i] != (bool)key[i])
      lastTicksKey[i] = key[i];
  }
}
