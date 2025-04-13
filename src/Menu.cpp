#include "Menu.h"

// Create menu
void Menu::init() {
  auto screenSize = asw::display::getLogicalSize();

  // Load images
  menu = asw::assets::loadTexture("assets/images/gui/menu.png");
  menuselect = asw::assets::loadTexture("assets/images/gui/menuSelector.png");
  help = asw::assets::loadTexture("assets/images/gui/help.png");
  cursor = asw::assets::loadTexture("assets/images/gui/cursor1.png");
  levelSelectNumber =
      asw::assets::loadTexture("assets/images/gui/levelSelectNumber.png");
  copyright = asw::assets::loadTexture("assets/images/gui/copyright.png");
  credits = asw::assets::loadTexture("assets/images/gui/credits.png");

  // Load sound
  click = asw::assets::loadSample("assets/sounds/click.wav");
  intro = asw::assets::loadSample("assets/sounds/intro.wav");
  music = asw::assets::loadSample("assets/sounds/music/MiniJim.ogg");

  // Sets Font
  menuFont = asw::assets::loadFont("assets/fonts/ariblk.ttf", 24);

  // Create map for live background
  levelOn = 0;
  tile_map = new TileMap();
  change_level(0);
  next_state = ProgramState::Null;

  // Buttons
  buttons[BUTTON_START] = Button(asw::Vec2<float>(60, 630));
  buttons[BUTTON_START_MP] = Button(asw::Vec2<float>(60, 690));
  buttons[BUTTON_EDIT] = Button(asw::Vec2<float>(60, 750));
  buttons[BUTTON_HELP] = Button(asw::Vec2<float>(60, 810));
  buttons[BUTTON_EXIT] = Button(asw::Vec2<float>(60, 870));
  buttons[BUTTON_LEFT] = Button(asw::Vec2<float>(screenSize.x - 180, 80));
  buttons[BUTTON_RIGHT] = Button(asw::Vec2<float>(screenSize.x - 80, 80));

  buttons[BUTTON_START].SetImages("assets/images/gui/button_start.png",
                                  "assets/images/gui/button_start_hover.png");
  buttons[BUTTON_START_MP].SetImages(
      "assets/images/gui/button_start_mp.png",
      "assets/images/gui/button_start_mp_hover.png");
  buttons[BUTTON_EDIT].SetImages("assets/images/gui/button_edit.png",
                                 "assets/images/gui/button_edit_hover.png");
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
    next_state = ProgramState::Game;
  });

  buttons[BUTTON_START_MP].SetOnClick([this]() {
    single_player = false;
    next_state = ProgramState::Game;
  });

  buttons[BUTTON_EDIT].SetOnClick(
      [this]() { next_state = ProgramState::Edit; });

  buttons[BUTTON_EXIT].SetOnClick(
      [this]() { next_state = ProgramState::Edit; });

  buttons[BUTTON_LEFT].SetOnClick([this]() { change_level(-1); });

  buttons[BUTTON_RIGHT].SetOnClick([this]() { change_level(1); });

  // Variables
  asw::sound::play(music, 255, 128, 1);
  asw::sound::play(intro);
}

void Menu::cleanup() {
  // Destory background
  delete tile_map;
}

void Menu::change_level(int level) {
  auto screenSize = asw::display::getLogicalSize();

  levelOn = (levelOn + level) < 0 ? 4 : (levelOn + level) % 5;

  tile_map->load("assets/data/level_" + std::to_string(levelOn + 1));

  scroll_x = static_cast<float>(
      random(screenSize.x, tile_map->getWidth() - screenSize.x));
  scroll_dir_x = static_cast<float>(random(0, 1) != 0 ? -3 : 3);
  scroll_y = static_cast<float>(
      random(screenSize.y, tile_map->getHeight() - screenSize.y));
  scroll_dir_y = static_cast<float>(random(0, 1) != 0 ? -3 : 3);

  asw::sound::play(click);

  cam = Camera(screenSize.x, screenSize.y, tile_map->getWidth(),
               tile_map->getHeight());
  cam.SetSpeed(5);
}

void Menu::update() {
  auto screenSize = asw::display::getLogicalSize();

  // Move around live background
  if (scroll_x + screenSize.x / 2 >= tile_map->getWidth() ||
      scroll_x <= screenSize.x / 2) {
    scroll_dir_x *= -1;
  }

  if (scroll_y + screenSize.y / 2 >= tile_map->getHeight() ||
      scroll_y <= screenSize.y / 2) {
    scroll_dir_y *= -1;
  }

  scroll_x += scroll_dir_x;
  scroll_y += scroll_dir_y;

  cam.Follow(scroll_x, scroll_y);

  // State change
  if (next_state != ProgramState::Null) {
    setNextState(next_state);
  }

  // Buttons
  for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].Update();
  }
}

void Menu::draw() {
  auto screenSize = asw::display::getLogicalSize();

  // Draw background to screen
  asw::draw::rectFill(asw::Quad<float>(0, 0, screenSize.x, screenSize.y),
                      asw::util::makeColor(255, 255, 255, 255));

  // Draw live background
  tile_map->draw(cam.GetX(), cam.GetY(), screenSize.x, screenSize.y);

  // Lighting
  if (tile_map->hasLighting()) {
    std::vector<SDL_Point> lightPoints;

    // Get map area
    std::vector<Tile*> mapRange =
        tile_map->get_tiles_in_range(cam.GetX(), cam.GetX() + cam.GetWidth(),
                                     cam.GetY(), cam.GetY() + cam.GetHeight());

    for (auto t : mapRange) {
      if (t->containsAttribute(light)) {
        lightPoints.push_back(
            {t->getCenterX() - cam.GetX(), t->getCenterY() - cam.GetY()});
      }
    }

    lightPoints.push_back({static_cast<int>(asw::input::mouse.x),
                           static_cast<int>(asw::input::mouse.y)});

    lightLayer.draw(lightPoints);
  }

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

  // Cursor
  asw::draw::sprite(cursor,
                    asw::Vec2<float>(asw::input::mouse.x, asw::input::mouse.y));

  // Help menu
  if (buttons[BUTTON_HELP].Hover()) {
    asw::draw::sprite(help, asw::Vec2<float>(0, 0));
  }

  asw::draw::sprite(copyright,
                    asw::Vec2<float>(screenSize.x - 350, screenSize.y - 40));

  asw::draw::sprite(cursor,
                    asw::Vec2<float>(asw::input::mouse.x, asw::input::mouse.y));
}
