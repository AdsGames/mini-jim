#ifndef INIT_H
#define INIT_H

#include "State.h"

class Init : public State {
 public:
  virtual void init() override;
  virtual void update(StateEngine& engine) override;
  virtual void draw() override;
  virtual void cleanup() override;
};

#endif  // INIT_H
