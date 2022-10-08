#ifndef INIT_H
#define INIT_H

#include "State.h"

class Init : public State {
 public:
  virtual ~Init();

  virtual void init();
  virtual void update(StateEngine& engine) override;
  virtual void draw() override;
};

#endif  // INIT_H
