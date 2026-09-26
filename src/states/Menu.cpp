#include "./Menu.h"

// Create menu
void Menu::init() {
  auto screenSize = asw::display::get_logical_size();

  // Load images
  menu = asw::assets::load_texture("assets/images/gui/menu.png");
  menuselect = asw::assets::load_texture("assets/images/gui/menuSelector.png");
  help = asw::assets::load_texture("assets/images/gui/help.png");
  levelSelectNumber =
      asw::assets::load_texture("assets/images/gui/levelSelectNumber.png");
  copyright = asw::assets::load_texture("assets/images/gui/copyright.png");
  credits = asw::assets::load_texture("assets/images/gui/credits.png");

  // Load sound
  click = asw::assets::load_sample("assets/sounds/click.wav");
  intro = asw::assets::load_sample("assets/sounds/intro.wav");
  music = asw::assets::load_music("assets/sounds/music/MiniJim.ogg");

  // Sets Font
  menuFont = asw::assets::load_font("assets/fonts/ariblk.ttf", 24);

  // Create map for live background
  levelOn = 0;
  tile_map = TileMap();
  change_level(0);
  next_state = ProgramState::Null;

  // Buttons
  buttons[BUTTON_START] = Button(asw::Vec2(60, 630));
  buttons[BUTTON_START_MP] = Button(asw::Vec2(60, 690));
  buttons[BUTTON_HELP] = Button(asw::Vec2(60, 810));
  buttons[BUTTON_EXIT] = Button(asw::Vec2(60, 870));
  buttons[BUTTON_LEFT] = Button(asw::Vec2(screenSize.x - 180, 80));
  buttons[BUTTON_RIGHT] = Button(asw::Vec2(screenSize.x - 80, 80));

  buttons[BUTTON_START].SetImages("assets/images/gui/button_start.png",
                                  "assets/images/gui/button_start_hover.png");
  buttons[BUTTON_START_MP].SetImages(
      "assets/images/gui/button_start_mp.png",
      "assets/images/gui/button_start_mp_hover.png");
  buttons[BUTTON_HELP].SetImages("assets/images/gui/button_help.png",
                                 "assets/images/gui/button_help_hover.png");
  buttons[BUTTON_EXIT].SetImages("assets/images/gui/button_quit.png",
                                 "assets/images/gui/button_quit_hover.png");
  buttons[BUTTON_LEFT].SetImages("assets/images/gui/button_left.png",
                                 "assets/images/gui/button_left_hover.png");
  buttons[BUTTON_RIGHT].SetImages("assets/images/gui/button_right.png",
                                  "assets/images/gui/button_right_hover.png");

  buttons[BUTTON_START].SetOnClick([this]() {
    single_player = true;
    manager.set_next_scene(ProgramState::Game);
  });

  buttons[BUTTON_START_MP].SetOnClick([this]() {
    single_player = false;
    manager.set_next_scene(ProgramState::Game);
  });

  buttons[BUTTON_EXIT].SetOnClick([]() { asw::core::exit(); });

  buttons[BUTTON_LEFT].SetOnClick([this]() { change_level(-1); });

  buttons[BUTTON_RIGHT].SetOnClick([this]() { change_level(1); });

  // Variables
  asw::sound::play_music(music);
  asw::sound::play(intro);
}

void Menu::change_level(int level) {
  auto screenSize = asw::display::get_logical_size();

  levelOn =
      (levelOn + level) < 0 ? (levelCount - 1) : (levelOn + level) % levelCount;

  tile_map.load("assets/levels/level_" + std::to_string(levelOn + 1) + ".json");

  scroll.x =
      asw::random::between(screenSize.x, tile_map.getWidth() - screenSize.x);
  scroll_dir.x = asw::random::chance(0.5F) ? -3 : 3;
  scroll.y =
      asw::random::between(screenSize.y, tile_map.getHeight() - screenSize.y);
  scroll_dir.y = asw::random::chance(0.5F) ? -3 : 3;

  asw::sound::play(click);

  cam = Camera(screenSize.x, screenSize.y, tile_map.getWidth(),
               tile_map.getHeight());
  cam.setSpeed(5);
}

void Menu::update(float dt) {
  // asw passes seconds; game logic is tuned in milliseconds
  dt *= 1000.0F;

  auto screenSize = asw::display::get_logical_size();

  // Move around live background
  if (scroll.x + screenSize.x / 2 >= tile_map.getWidth() ||
      scroll.x <= screenSize.x / 2) {
    scroll_dir.x *= -1;
  }

  if (scroll.y + screenSize.y / 2 >= tile_map.getHeight() ||
      scroll.y <= screenSize.y / 2) {
    scroll_dir.y *= -1;
  }

  scroll += (scroll_dir / 16.0F) * dt;

  cam.follow(scroll, dt);

  // Buttons
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].Update();
  }

  // Tile
  tile_map.update(dt);
}

void Menu::draw() {
  auto screenSize = asw::display::get_logical_size();

  // Draw live background
  tile_map.draw(cam.getViewport(), 0, 0, 1);
  tile_map.drawShadows(cam.getViewport(), 0, 0);
  tile_map.draw(cam.getViewport(), 0, 0, 2);
  tile_map.drawLights(cam.getViewport(), 0, 0);

  // Overlay
  asw::draw::sprite(credits, asw::Vec2(0, 0));
  asw::draw::sprite(menu, asw::Vec2(40, 480));

  // Buttons
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].Draw();
  }

  // Level selection
  asw::draw::sprite(levelSelectNumber, asw::Vec2(screenSize.x - 160, 80));
  asw::draw::text(menuFont, std::to_string(levelOn + 1),
                  asw::Vec2(screenSize.x - 120, 80), asw::Color(0, 0, 0));

  // Help menu
  if (buttons[BUTTON_HELP].Hover()) {
    asw::draw::sprite(help, asw::Vec2(0, 0));
  }

  asw::draw::sprite(copyright,
                    asw::Vec2(screenSize.x - 350, screenSize.y - 40));
}
