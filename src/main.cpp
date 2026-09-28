#include <asw/asw.h>

#include "./Controls.h"

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
  controls::bind_ui();

  auto app = asw::scene::SceneManager<ProgramState>();
  app.register_scene<Init>(ProgramState::Init, app);
  app.register_scene<Intro>(ProgramState::Intro, app);
  app.register_scene<Menu>(ProgramState::Menu, app);
  app.register_scene<Game>(ProgramState::Game, app);
  app.set_next_scene(ProgramState::Init);

  app.start();

  return 0;
}
