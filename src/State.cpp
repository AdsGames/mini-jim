#include "State.h"

#include <iostream>

#include "Game.h"
#include "Menu.h"
#include "Editor.h"
#include "Init.h"
#include "Intro.h"

/*****************
 * STATE ENGINE
 *****************/

// Init
StateEngine::StateEngine()
  : nextState(STATE_NULL), currentState(STATE_NULL), state(nullptr) {

}

// Draw
void StateEngine::draw (BITMAP *buffer) {
  if (state) {
    state -> draw (buffer);
  }
}

// Update
void StateEngine::update() {
  if (state) {
    state -> update (*this);
  }

  changeState();
}

// Set next state
void StateEngine::setNextState (const int newState) {
  nextState = newState;
}

// Get state id
int StateEngine::getStateId() const {
  return currentState;
}

// Change game screen
void StateEngine::changeState() {
  // If the state needs to be changed
  if (nextState == STATE_NULL) {
    return;
  }

  // Delete the current state
  if (state) {
    delete state;
    state = nullptr;
  }


  // Change the state
  switch (nextState) {
    case STATE_GAME:
      state = new Game();
      std::cout << ("Switched state to game.\n");
      break;

    case STATE_MENU:
      state = new Menu();
      std::cout << ("Switched state to main menu.\n");
      break;

    case STATE_INIT:
      state = new Init();
      std::cout << ("Switched state to init.\n");
      break;

    case STATE_INTRO:
      state = new Intro();
      std::cout << ("Switched state to intro.\n");
      break;

    case STATE_EDIT:
      state = new Editor();
      std::cout << ("Switched state to edit.\n");
      break;

    default:
      std::cout << ("Exiting program.");
      break;
  }

  // Change the current state ID
  currentState = nextState;

  // NULL the next state ID
  nextState = STATE_NULL;
}


/*********
 * STATE
 *********/

// Change state
void State::setNextState (StateEngine &engine, int state) {
  engine.setNextState (state);
}
