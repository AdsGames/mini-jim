#include <asw/asw.h>

// For state engine
#include "./states/Game.h"
#include "./states/Init.h"
#include "./states/Intro.h"
#include "./states/Menu.h"
#include "./states/State.h"

// Main function*/
int main() {
  // Load allegro library
  asw::core::init(1280, 960);

  auto app = asw::scene::SceneManager<ProgramState>();
  app.registerScene<Init>(ProgramState::Init, app);
  app.registerScene<Intro>(ProgramState::Intro, app);
  app.registerScene<Menu>(ProgramState::Menu, app);
  app.registerScene<Game>(ProgramState::Game, app);
  app.setNextScene(ProgramState::Init);

  app.start();

  return 0;
}
