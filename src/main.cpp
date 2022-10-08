// Includes
#include <chrono>
#include "./lib/aar/aar.h"

#include "utility/JoystickListener.h"
#include "utility/KeyListener.h"
#include "utility/MouseListener.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

// For state engine
#include "State.h"
#include "globals.h"

using namespace std::chrono_literals;
using namespace std::chrono;
constexpr nanoseconds timestep(16ms);

// State engine
StateEngine* game_state;

// Buffer
SDL_Window* gWindow = nullptr;

// Functions
// void close_button_handler(void);
void setup();
void draw();
void update();

// Close button handler
bool close_button_pressed = false;
bool closeGame;

// FPS system
int fps = 0;
int frames_done = 0;

// Setup game
void setup() {
  // Load allegro library
  aar::util::init();

  gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, SCREEN_W, SCREEN_H,
                             SDL_WINDOW_SHOWN);

  if (!gWindow) {
    aar::util::abortOnError("WINDOW");
  }

  // Get window surface
  aar::renderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

  SDL_SetRenderDrawBlendMode(aar::renderer, SDL_BLENDMODE_BLEND);

  // Variables
  closeGame = false;

  game_state = new StateEngine(gWindow);
}

// Update
void update() {
  // Update listeners
  KeyListener::update();
  MouseListener::update();
  JoystickListener::update();

  // Do state logic
  game_state->update();

  // Handle exit
  if (game_state->getStateId() == StateEngine::STATE_EXIT) {
    close_button_pressed = true;
  }
}

// Do state rendering
void draw() {
  game_state->draw(aar::renderer);
  // aar::draw::stretchSprite(screenRenderer, buffer, 0, 0, SCREEN_W, SCREEN_H);
  SDL_UpdateWindowSurface(gWindow);
}

// Loop (emscripten compatibility)
#ifdef __EMSCRIPTEN__
void loop() {
  update();
  draw();
}
#endif

// Main function*/
int main(int argc, char* argv[]) {
  // Setup basic functionality
  setup();

  // Set the current state ID
  game_state->setNextState(StateEngine::STATE_INIT);

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(loop, 0, 1);
#else

  using clock = high_resolution_clock;
  nanoseconds lag(0ns);
  auto time_start = clock::now();

  while (!KeyListener::keyDown[SDL_SCANCODE_ESCAPE] && !close_button_pressed) {
    auto delta_time = clock::now() - time_start;
    time_start = clock::now();
    lag += duration_cast<nanoseconds>(delta_time);

    while (lag >= timestep) {
      lag -= timestep;
      update();
    }

    draw();
    frames_done++;
  }
#endif

  return 0;
}
