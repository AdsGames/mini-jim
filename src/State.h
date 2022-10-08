/**
 * State for machine and State Engine
 * Allan Legemaate
 * 30/12/2016
 * Compartmentalize program into states
 *   which can handle only their own logic,
 *   drawing and transitions
 */

#ifndef STATE_H
#define STATE_H

#include "./lib/aar/aar.h"

// Class
class State;

/*****************
 * STATE ENGINE
 *****************/
class StateEngine {
 public:
  // Init
  StateEngine();

  // Update
  void update();

  // Draw
  void draw();

  // Set next state
  void setNextState(const int newState);

  // Get state id
  int getStateId() const;

  // Game states
  enum programStates {
    STATE_NULL,
    STATE_INIT,
    STATE_INTRO,
    STATE_MENU,
    STATE_EDIT,
    STATE_GAME,
    STATE_EXIT,
  };

 private:
  // Change state
  void changeState();

  // Next state
  int nextState = STATE_NULL;

  // State id
  int currentState = STATE_NULL;

  // Stores states
  State* state;
};

/*********
 * STATE
 *********/
class State {
 public:
  // Init the state
  virtual void init() = 0;

  // Draw to screen
  virtual void draw() = 0;

  // Cleanup
  virtual void cleanup() = 0;

  // Update logic
  virtual void update(StateEngine& engine) = 0;

  // Change state
  static void setNextState(StateEngine& engine, int state);
};

#endif  // STATE_H
