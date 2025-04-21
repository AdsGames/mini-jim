#include "./Menu.h"

// Create menu
void Menu::init() {
  auto screenSize = asw::display::getLogicalSize();

  // Load images
  menu = asw::assets::loadTexture("assets/images/gui/menu.png");
  menuselect = asw::assets::loadTexture("assets/images/gui/menuSelector.png");
  help = asw::assets::loadTexture("assets/images/gui/help.png");
  levelSelectNumber =
      asw::assets::loadTexture("assets/images/gui/levelSelectNumber.png");
  copyright = asw::assets::loadTexture("assets/images/gui/copyright.png");
  credits = asw::assets::loadTexture("assets/images/gui/credits.png");

  // Load sound
  click = asw::assets::loadSample("assets/sounds/click.wav");
  intro = asw::assets::loadSample("assets/sounds/intro.wav");
  music = asw::assets::loadMusic("assets/sounds/music/MiniJim.ogg");

  // Sets Font
  menuFont = asw::assets::loadFont("assets/fonts/ariblk.ttf", 24);

  // Create map for live background
  levelOn = 0;
  tile_map = TileMap();
  change_level(0);
  next_state = ProgramState::Null;

  // Buttons
  buttons[BUTTON_START] = Button(asw::Vec2<float>(60, 630));
  buttons[BUTTON_START_MP] = Button(asw::Vec2<float>(60, 690));
  buttons[BUTTON_HELP] = Button(asw::Vec2<float>(60, 810));
  buttons[BUTTON_EXIT] = Button(asw::Vec2<float>(60, 870));
  buttons[BUTTON_LEFT] = Button(asw::Vec2<float>(screenSize.x - 180, 80));
  buttons[BUTTON_RIGHT] = Button(asw::Vec2<float>(screenSize.x - 80, 80));

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
    sceneManager.setNextScene(ProgramState::Game);
  });

  buttons[BUTTON_START_MP].SetOnClick([this]() {
    single_player = false;
    sceneManager.setNextScene(ProgramState::Game);
  });

  buttons[BUTTON_EXIT].SetOnClick([]() { asw::core::exit = true; });

  buttons[BUTTON_LEFT].SetOnClick([this]() { change_level(-1); });

  buttons[BUTTON_RIGHT].SetOnClick([this]() { change_level(1); });

  // Variables
  asw::sound::playMusic(music, 255);
  asw::sound::play(intro);
}

void Menu::change_level(int level) {
  auto screenSize = asw::display::getLogicalSize();

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
  auto screenSize = asw::display::getLogicalSize();

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
  auto screenSize = asw::display::getLogicalSize();

  // Draw live background
  tile_map.draw(cam.getViewport(), 0, 0, 1);
  tile_map.drawShadows(cam.getViewport(), 0, 0);
  tile_map.draw(cam.getViewport(), 0, 0, 2);
  tile_map.drawLights(cam.getViewport(), 0, 0);

  // Overlay
  asw::draw::sprite(credits, asw::Vec2<float>(0, 0));
  asw::draw::sprite(menu, asw::Vec2<float>(40, 480));

  // Buttons
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].Draw();
  }

  // Level selection
  asw::draw::sprite(levelSelectNumber,
                    asw::Vec2<float>(screenSize.x - 160, 80));
  asw::draw::text(menuFont, std::to_string(levelOn + 1),
                  asw::Vec2<float>(screenSize.x - 120, 80),
                  asw::util::makeColor(0, 0, 0));

  // Help menu
  if (buttons[BUTTON_HELP].Hover()) {
    asw::draw::sprite(help, asw::Vec2<float>(0, 0));
  }

  asw::draw::sprite(copyright,
                    asw::Vec2<float>(screenSize.x - 350, screenSize.y - 40));
}
