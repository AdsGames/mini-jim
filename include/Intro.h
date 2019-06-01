#ifndef INTRO_H
#define INTRO_H

#include <allegro.h>

#include "State.h"
#include "utility/Timer.h"

#define INTRO_FRAMES 84

// Intro screen of game
class Intro : public State {
  public:
    Intro();
    virtual ~Intro();
    virtual void update(StateEngine *engine) override;
    virtual void draw(BITMAP *buffer) override;

  private:
    BITMAP *intro;
    BITMAP *title;
    BITMAP *background;
    BITMAP *images[INTRO_FRAMES];
    SAMPLE *introSound;

    int frame;
    bool sound_played;

    Timer timer;
};

#endif // INTRO_H
