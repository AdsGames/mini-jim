#ifndef INIT_H
#define INIT_H

#include "GameState.h"

class Init : public GameState {
  private:
  protected:
  public:
    //Main loop functions
    Init();
    void update();
    void draw();
    ~Init();
};

#endif // INIT_H
