#include "State.h"

#include <iostream>

#include "Editor.h"
#include "Game.h"
#include "Init.h"
#include "Intro.h"
#include "Menu.h"

/*****************
 * STATE ENGINE
 *****************/

// Draw
void StateEngine::draw() {
  if (state) {
    // Clear screen
    SDL_RenderClear(asw::display::renderer);

    state->draw();

    // Update screen
    SDL_RenderPresent(asw::display::renderer);
  }
}

// Update
void StateEngine::update() {
  if (state) {
    state->update();
  }

  changeState();
}

// Set next state
void StateEngine::setNextState(const ProgramState newState) {
  nextState = newState;
}

// Get state id
auto StateEngine::getStateId() const -> ProgramState {
  return currentState;
}

// Change game screen
void StateEngine::changeState() {
  // If the state needs to be changed
  if (nextState == ProgramState::Null) {
    return;
  }

  // Delete the current state
  if (state) {
    state->cleanup();
    state = nullptr;
  }

  // Change the state
  switch (nextState) {
    case ProgramState::Game:
      state = std::make_unique<Game>(*this);
      std::cout << ("Switched state to game.\n");
      break;

    case ProgramState::Menu:
      state = std::make_unique<Menu>(*this);
      std::cout << ("Switched state to main menu.\n");
      break;

    case ProgramState::Init:
      state = std::make_unique<Init>(*this);
      std::cout << ("Switched state to init.\n");
      break;

    case ProgramState::Intro:
      state = std::make_unique<Intro>(*this);
      std::cout << ("Switched state to intro.\n");
      break;

    case ProgramState::Edit:
      state = std::make_unique<Editor>(*this);
      std::cout << ("Switched state to edit.\n");
      break;

    default:
      std::cout << ("Exiting program.");
      break;
  }

  state->init();

  // Change the current state ID
  currentState = nextState;

  // NULL the next state ID
  nextState = ProgramState::Null;
}

/*********
 * STATE
 *********/

// Change state
void State::setNextState(const ProgramState state) {
  this->engine.setNextState(state);
}
