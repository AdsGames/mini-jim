#ifndef INTRO_H
#define INTRO_H

#include "State.h"

#include <allegro.h>

#define INTRO_FRAMES 82

// Intro screen of game
class Intro : public State {
  public:
    Intro();
    virtual ~Intro();
    virtual void update(StateEngine *engine) override;
    virtual void draw(BITMAP *buffer) override;

  private:
    BITMAP *buffer;
    BITMAP *intro;
    BITMAP *title;
    BITMAP *background;
    BITMAP *images[INTRO_FRAMES];

    SAMPLE *introSound;
};

#endif // INTRO_H
