#ifndef INTRO_H
#define INTRO_H

#include "GameState.h"

#include <allegro.h>

#define INTRO_FRAMES 82

// Intro screen of game
class Intro : public GameState {
  public:
    Intro();
    void update();
    void draw();
    ~Intro();

  private:
    BITMAP *buffer;
    BITMAP *intro;
    BITMAP *title;
    BITMAP *background;
    BITMAP *images[INTRO_FRAMES];

    SAMPLE *introSound;
};

#endif // INTRO_H
