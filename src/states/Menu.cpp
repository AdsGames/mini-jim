#include "./Menu.h"

#include <cmath>
#include <functional>
#include <string>
#include <utility>

#include "../Controls.h"

namespace {
// How quickly the background camera catches up with the scroll point
constexpr float CAMERA_FOLLOW_SPEED = 64.0F;
}  // namespace

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

  // Buttons, init runs again each time the menu is entered
  ui.root.clear_children();
  ui.clear_focus();

  // Controller navigation uses the game's menu actions. Left and right change
  // the level, so they never move focus off the menu list.
  ui.ctx.navigation.up = controls::UI_UP;
  ui.ctx.navigation.down = controls::UI_DOWN;
  ui.ctx.navigation.left = controls::UI_LEFT;
  ui.ctx.navigation.right = controls::UI_RIGHT;
  ui.ctx.navigation.activate = controls::UI_CONFIRM;
  ui.ctx.navigation.back = controls::UI_BACK;

  const auto add_button = [this](const std::string& name, asw::Vec2f position,
                                 std::function<void()> on_click) {
    auto& button = ui.root.add_child<asw::ui::Button>();
    const std::string path = "assets/images/gui/button_" + name;
    button.set_images(asw::assets::load_texture(path + ".png"),
                      asw::assets::load_texture(path + "_hover.png"));
    button.transform.position = position;
    button.focus_ring = false;
    button.on_click = std::move(on_click);
    return &button;
  };

  auto* start = add_button("start", asw::Vec2f(60, 630), [this]() {
    single_player = true;
    manager.set_next_scene(ProgramState::Game);
  });

  auto* start_mp = add_button("start_mp", asw::Vec2f(60, 690), [this]() {
    single_player = false;
    manager.set_next_scene(ProgramState::Game);
  });

  // Shows the help overlay while highlighted
  help_button = add_button("help", asw::Vec2f(60, 810), nullptr);

  auto* exit = add_button("quit", asw::Vec2f(60, 870),
                          []() { asw::core::exit(); });

  // Level arrows, clear focus after a click so the controller starts on the
  // menu list again
  add_button("left", asw::Vec2f(screenSize.x - 180, 80), [this]() {
    change_level(-1);
    ui.clear_focus();
  });
  add_button("right", asw::Vec2f(screenSize.x - 80, 80), [this]() {
    change_level(1);
    ui.clear_focus();
  });

  // The menu list wraps, and left or right keeps focus in place
  start->nav_up = exit;
  exit->nav_down = start;
  for (auto* button : {start, start_mp, help_button, exit}) {
    button->nav_left = button;
    button->nav_right = button;
  }

  ui.ctx.focus.default_focus = start;

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

  cam = asw::Camera(asw::Vec2f(static_cast<float>(screenSize.x),
                               static_cast<float>(screenSize.y)));
  cam.set_bounds(asw::Quadf(0.0F, 0.0F, static_cast<float>(tile_map.getWidth()),
                            static_cast<float>(tile_map.getHeight())));
  cam.set_follow_speed(CAMERA_FOLLOW_SPEED);
  cam.snap_to(scroll);
}

void Menu::update(float dt) {
  // asw runs a fixed timestep and passes seconds; the live background is tuned
  // in milliseconds
  const float dt_ms = dt * 1000.0F;
  auto screenSize = asw::display::get_logical_size();

  // Move around live background, always bounce back inward so a scroll that
  // overshoots an edge cannot flip direction every step and get stuck
  if (scroll.x + screenSize.x / 2 >= tile_map.getWidth()) {
    scroll_dir.x = -std::abs(scroll_dir.x);
  } else if (scroll.x <= screenSize.x / 2) {
    scroll_dir.x = std::abs(scroll_dir.x);
  }

  if (scroll.y + screenSize.y / 2 >= tile_map.getHeight()) {
    scroll_dir.y = -std::abs(scroll_dir.y);
  } else if (scroll.y <= screenSize.y / 2) {
    scroll_dir.y = std::abs(scroll_dir.y);
  }

  scroll += (scroll_dir / 16.0F) * dt_ms;

  cam.follow(scroll, dt);

  // Tile
  tile_map.update(dt_ms);

  // Controller players get a focused button instead of the mouse cursor
  asw::input::set_cursor_visible(asw::input::get_last_device() !=
                                 asw::input::InputDevice::Controller);

  ui.update();

  if (asw::input::get_action_down(controls::UI_LEFT)) {
    change_level(-1);
  }

  if (asw::input::get_action_down(controls::UI_RIGHT)) {
    change_level(1);
  }
}

void Menu::draw() {
  auto screenSize = asw::display::get_logical_size();

  // Draw live background
  tile_map.draw(cam.get_view(), 0, 0, 1);
  tile_map.drawShadows(cam.get_view(), 0, 0);
  tile_map.draw(cam.get_view(), 0, 0, 2);
  tile_map.drawLights(cam.get_view(), 0, 0);

  // Overlay
  asw::draw::sprite(credits, asw::Vec2f(0, 0));
  asw::draw::sprite(menu, asw::Vec2f(40, 480));

  // Buttons
  ui.draw();

  // Level selection
  asw::draw::sprite(levelSelectNumber, asw::Vec2f(screenSize.x - 160, 80));
  asw::draw::text(menuFont, std::to_string(levelOn + 1),
                  asw::Vec2f(screenSize.x - 120, 80), asw::Color(0, 0, 0));

  // Help menu
  if (help_button != nullptr && help_button->is_highlighted(ui.ctx)) {
    asw::draw::sprite(help, asw::Vec2f(0, 0));
  }

  asw::draw::sprite(copyright,
                    asw::Vec2f(screenSize.x - 350, screenSize.y - 40));
}
