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
  StateEngine(aar::Window* window);

  // Update
  void update();

  // Draw
  void draw(aar::Renderer* buffer);

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

  // Window
  aar::Window* window;
};

/*********
 * STATE
 *********/
class State {
 public:
  // Virtual destructor
  State(){};
  virtual ~State(){};

  // Init the state
  virtual void init(aar::Window* window) = 0;

  // Draw to screen
  virtual void draw(aar::Renderer* buffer) = 0;

  // Update logic
  virtual void update(StateEngine& engine) = 0;

  // Change state
  static void setNextState(StateEngine& engine, int state);
};

#endif  // STATE_H
