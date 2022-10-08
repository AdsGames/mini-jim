#ifndef INTRO_H
#define INTRO_H

#include "./lib/aar/aar.h"

#include "State.h"
#include "utility/Timer.h"

#define INTRO_FRAMES 84

// Intro screen of game
class Intro : public State {
 public:
  virtual ~Intro();

  virtual void init(aar::Window* window);
  virtual void update(StateEngine& engine) override;
  virtual void draw(aar::Renderer* buffer) override;

 private:
  aar::Texture* intro;
  aar::Texture* title;
  aar::Texture* background;
  aar::Texture* images[INTRO_FRAMES];
  aar::Sample* introSound;

  int frame;
  bool sound_played;

  Timer timer;
};

#endif  // INTRO_H
