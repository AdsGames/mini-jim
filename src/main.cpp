
// Includes
#include <allegro.h>
#include <chrono>

#include "utility/KeyListener.h"
#include "utility/MouseListener.h"
#include "utility/JoystickListener.h"

// For state engine
#include "State.h"
#include "globals.h"

using namespace std::chrono_literals;
using namespace std::chrono;
constexpr nanoseconds timestep(16ms);

// State engine
StateEngine game_state;

// Buffer
BITMAP *buffer;

KeyListener keyL;
MouseListener mouseL;
JoystickListener joyL;

// Close button handler
volatile int close_button_pressed = FALSE;
bool closeGame;

void close_button_handler (void) {
  close_button_pressed = TRUE;
}
END_OF_FUNCTION (close_button_handler)

// FPS system
int fps = 0;
int frames_done = 0;

// Setup game
void setup() {
  // Load allegro library
  allegro_init();
  install_timer();
  install_keyboard();
  install_mouse();
  install_joystick (JOY_TYPE_AUTODETECT);
  install_sound (DIGI_AUTODETECT, MIDI_AUTODETECT, ".");
  set_color_depth (32);

  // Close button
  LOCK_FUNCTION (close_button_handler);
  set_close_button_callback (close_button_handler);

  // Variables
  closeGame = false;

  // Create screen
  if (set_gfx_mode (GFX_AUTODETECT_WINDOWED, NATIVE_SCREEN_W, NATIVE_SCREEN_H, 0, 0)) {
    if (set_gfx_mode (GFX_AUTODETECT_WINDOWED, NATIVE_SCREEN_W / 2, NATIVE_SCREEN_H / 2, 0, 0)) {
      set_gfx_mode (GFX_TEXT, 0, 0, 0, 0);
      allegro_message ("Could not create screen. Exiting", -1, allegro_error);
      exit (-1);
    }
  }

  // Create global buffer
  buffer = create_bitmap(NATIVE_SCREEN_W, NATIVE_SCREEN_H);
}

// Update
void update() {
  // Update listeners
  keyL.update();
  mouseL.update();
  joyL.update();

  //Do state logic
  game_state.update();

  // Handle exit
  if (game_state.getStateId() == StateEngine::STATE_EXIT)
    close_button_pressed = true;
}

//Do state rendering
void draw() {
  game_state.draw(buffer);
  stretch_sprite(screen, buffer, 0, 0, SCREEN_W, SCREEN_H);
}

//Main function*/
int main() {
  // Setup basic functionality
  setup();

  //Set the current state ID
  game_state.setNextState(StateEngine::STATE_INIT);

  using clock = high_resolution_clock;
  nanoseconds lag(0ns);
  auto time_start = clock::now();

  while (!key[KEY_ESC] && !close_button_pressed) {
    auto delta_time = clock::now() - time_start;
    time_start = clock::now();
    lag += duration_cast<nanoseconds>(delta_time);

    while(lag >= timestep) {
      lag -= timestep;
      update();
    }

    draw();
    frames_done++;
  }

  return 0;
}
END_OF_MAIN()
