#ifndef INTRO_H
#define INTRO_H

#include "GameState.h"

#include <allegro.h>
#include <alpng.h>
#include <string>
#include <vector>
#include <string>

#include "globals.h"
#include "tools.h"

// Intro screen of game
class Intro : public GameState
{
  private:
    //Intro background
    BITMAP *buffer;
    BITMAP *intro;
    BITMAP *title;
    BITMAP *background;
    BITMAP *images[100];

    SAMPLE *introSound;

  public:
    //Main loop functions
    Intro();
    void update();
    void draw();
    ~Intro();
};

#endif // INTRO_H
