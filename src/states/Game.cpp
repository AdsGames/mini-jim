#include "./Game.h"

#include <algorithm>
#include <cmath>
#include <format>
#include <string>
#include <vector>

#include "../Controls.h"
#include "../globals.h"

namespace {
// How quickly cameras catch up with their player, higher is faster
constexpr float CAMERA_FOLLOW_SPEED = 36.0F;

// Screen shake in pixels when a player dies
constexpr float DEATH_SHAKE = 12.0F;

// HUD text and its backing
const asw::Color HUD_TEXT(255, 255, 255);
const asw::Color HUD_BACKING(0, 0, 0, 140);

// Format seconds to the nearest tenth
std::string format_time(double seconds) {
  return std::format("{:.1f}", seconds);
}

// HUD text with a shadow so it reads over any tile
void hud_text(const asw::Font& font,
              const std::string& text,
              const asw::Vec2f& position) {
  asw::draw::text_shadow(font, text, position, HUD_TEXT);
}

// Update a player and shake its camera if it died
void update_player(Player& player,
                   TileMap& tile_map,
                   asw::Camera& camera,
                   float dt_ms) {
  const int deaths = player.getDeathcount();
  player.update(tile_map, camera, dt_ms);

  if (player.getDeathcount() != deaths) {
    camera.shake(DEATH_SHAKE);
  }
}
}  // namespace

void Game::init() {
  // Player
  // Alone, any controller drives player 1. Together, each takes a controller.
  player1 = Player(1, single_player ? asw::input::ANY_CONTROLLER : 0);
  player2 = Player(2, 1);

  // Sets Font
  cooper = asw::assets::load_font("assets/fonts/cooper.ttf", 24);

  // Load images
  countdownImage = asw::assets::load_texture("assets/images/321go.png");

  results = asw::assets::load_texture("assets/images/gui/winscreen.png");
  results_singleplayer =
      asw::assets::load_texture("assets/images/gui/winscreen_singleplayer.png");

  // Samples
  countdown = asw::assets::load_sample("assets/sounds/countdown.wav");
  timeout = asw::assets::load_sample("assets/sounds/timeout.wav");

  // Load music
  mainMusic = asw::assets::load_music("assets/sounds/music/BasicJimFull.ogg");

  // Init
  setup();
}

void Game::setup() {
  tile_map = TileMap();

  const std::string file_name =
      "assets/levels/level_" + std::to_string(levelOn + 1) + ".json";

  if (!tile_map.load(file_name)) {
    asw::util::abort_on_error("Could not open level" + file_name);
  }

  auto screenSize = asw::display::get_logical_size();

  // Split screen gives each camera half the height
  const asw::Vec2f view_size(
      static_cast<float>(screenSize.x),
      static_cast<float>(single_player ? screenSize.y : screenSize.y / 2));
  const asw::Quadf world(0.0F, 0.0F, static_cast<float>(tile_map.getWidth()),
                         static_cast<float>(tile_map.getHeight()));

  for (auto* cam : {&cam_1, &cam_2}) {
    *cam = asw::Camera(view_size);
    cam->set_bounds(world);
    cam->set_follow_speed(CAMERA_FOLLOW_SPEED);
  }

  // Find spawn
  Tile* spawnTile = tile_map.find_tile_type(199, 1);

  if (spawnTile != nullptr) {
    player1.setSpawn(spawnTile->getTransform().position);
    player2.setSpawn(spawnTile->getTransform().position);
  }

  cam_1.snap_to(player1.getTransform().get_center());
  cam_2.snap_to(player2.getTransform().get_center());

  // Play music
  asw::sound::play(countdown);
  asw::sound::play_music(mainMusic);

  // Start game
  tm_begin.start();
}

void Game::update(float dt) {
  // asw runs a fixed timestep and passes seconds; game logic is tuned in
  // milliseconds
  const float dt_ms = dt * 1000.0F;

  // Camera follow
  cam_1.follow(player1.getTransform().get_center(), dt);
  cam_2.follow(player2.getTransform().get_center(), dt);
  cam_1.update(dt);
  cam_2.update(dt);

  // Tile
  tile_map.update(dt_ms);

  // Starting countdown
  if (!tm_begin.isRunning()) {
    // Stop from moving once done
    if (!player1.getFinished()) {
      update_player(player1, tile_map, cam_1, dt_ms);
    }

    if (!player2.getFinished() && !single_player) {
      update_player(player2, tile_map, cam_2, dt_ms);
    }
  }

  // Timers
  if (tm_begin.isRunning() &&
      tm_begin.getElapsedTime<std::chrono::milliseconds>() > 1200) {
    tm_begin.stop();
    tm_p1.start();
    tm_p2.start();
  }

  if (tm_p1.isRunning() && player1.getFinished()) {
    tm_p1.stop();
  }

  if (tm_p2.isRunning() && player2.getFinished()) {
    tm_p2.stop();
  }

  // Change level when both are done
  if (asw::input::get_action_down(controls::UI_CONFIRM) &&
      player1.getFinished() && (player2.getFinished() || single_player)) {
    manager.set_next_scene(ProgramState::Menu);
  }

  // Back to menu
  if (asw::input::get_action_down(controls::UI_BACK)) {
    manager.set_next_scene(ProgramState::Menu);
  }
}

void Game::draw() {
  auto screenSize = asw::display::get_logical_size();

  // Players carry a halo on dark levels
  std::vector<asw::Vec2f> halos{player1.getTransform().get_center()};
  if (!single_player) {
    halos.push_back(player2.getTransform().get_center());
  }

  // Draw tiles and characters
  if (single_player) {
    tile_map.draw(cam_1.get_view(), 0, 0, 1);
    player1.draw(cam_1.get_view().position);
    tile_map.drawShadows(cam_1.get_view(), 0, 0);
    tile_map.draw(cam_1.get_view(), 0, 0, 2);
    tile_map.drawLights(cam_1.get_view(), 0, 0, halos);
  } else {
    // Clip to remove interference
    SDL_Rect clip;
    clip.x = 0;
    clip.w = screenSize.x;

    // Top
    clip.y = 0;
    clip.h = screenSize.y / 2;

    SDL_SetRenderClipRect(asw::display::get_renderer(), &clip);
    tile_map.draw(cam_1.get_view(), 0, 0, 1);

    player1.draw(cam_1.get_view().position);
    player2.draw(cam_1.get_view().position);

    tile_map.drawShadows(cam_1.get_view(), 0, 0);
    tile_map.draw(cam_1.get_view(), 0, 0, 2);
    tile_map.drawLights(cam_1.get_view(), 0, 0, halos);

    // Bottom
    clip.y = screenSize.y / 2;
    clip.h = screenSize.y / 2;

    SDL_SetRenderClipRect(asw::display::get_renderer(), &clip);
    tile_map.draw(cam_2.get_view(), 0, screenSize.y / 2, 1);

    player1.draw(cam_2.get_view().position + asw::Vec2f(0, -screenSize.y / 2));
    player2.draw(cam_2.get_view().position + asw::Vec2f(0, -screenSize.y / 2));

    tile_map.drawShadows(cam_2.get_view(), 0, screenSize.y / 2);
    tile_map.draw(cam_2.get_view(), 0, screenSize.y / 2, 2);
    tile_map.drawLights(cam_2.get_view(), 0, screenSize.y / 2, halos);

    SDL_SetRenderClipRect(asw::display::get_renderer(), nullptr);
  }

  // Frame
  asw::draw::rect_fill(asw::Quadf(0, 0, screenSize.x, 16), asw::Color(0, 0, 0));
  asw::draw::rect_fill(asw::Quadf(0, 0, 16, screenSize.y), asw::Color(0, 0, 0));
  asw::draw::rect_fill(
      asw::Quadf(screenSize.x - 16, 0, screenSize.x, screenSize.y),
      asw::Color(0, 0, 0));
  asw::draw::rect_fill(
      asw::Quadf(0, screenSize.y - 16, screenSize.x, screenSize.y),
      asw::Color(0, 0, 0));

  // Timers
  asw::draw::rect_fill(asw::Quadf(20, 20, 320, 90), HUD_BACKING);

  if (!single_player) {
    asw::draw::rect_fill(asw::Quadf(20, (screenSize.y / 2) + 20, 320, 90),
                         HUD_BACKING);
  }

  // Draw timer to screen
  const auto timer1 =
      std::round(tm_p1.getElapsedTime<std::chrono::milliseconds>() / 100) / 10;
  const auto timer2 =
      std::round(tm_p2.getElapsedTime<std::chrono::milliseconds>() / 100) / 10;

  hud_text(cooper, "Time: " + format_time(timer1), asw::Vec2f(40, 55));

  hud_text(cooper, "Deaths:" + std::to_string(player1.getDeathcount()),
           asw::Vec2f(40, 20));

  if (!single_player) {
    hud_text(cooper, "Time: " + format_time(timer2),
             asw::Vec2f(40, (screenSize.y / 2) + 20 + 35));

    hud_text(cooper, "Deaths:" + std::to_string(player2.getDeathcount()),
             asw::Vec2f(40, (screenSize.y / 2) + 20));
  }

  // Starting countdown
  else {
    // Timer 3..2..1..GO!
    if (tm_begin.getElapsedTime<std::chrono::milliseconds>() < 330) {
      asw::draw::stretch_sprite_blit(
          countdownImage, asw::Quadf(0, 0, 14, 18),
          asw::Quadf(screenSize.x / 2 - 100, screenSize.y / 2 - 100, 140, 180));
    } else if (tm_begin.getElapsedTime<std::chrono::milliseconds>() < 660) {
      asw::draw::stretch_sprite_blit(
          countdownImage, asw::Quadf(19, 0, 14, 18),
          asw::Quadf(screenSize.x / 2 - 100, screenSize.y / 2 - 100, 140, 180));
    } else if (tm_begin.getElapsedTime<std::chrono::milliseconds>() < 990) {
      asw::draw::stretch_sprite_blit(
          countdownImage, asw::Quadf(39, 0, 14, 18),
          asw::Quadf(screenSize.x / 2 - 100, screenSize.y / 2 - 100, 140, 180));
    } else if (tm_begin.getElapsedTime<std::chrono::milliseconds>() < 1200) {
      asw::draw::stretch_sprite_blit(
          countdownImage, asw::Quadf(57, 0, 40, 18),
          asw::Quadf(screenSize.x / 2 - 200, screenSize.y / 2 - 100, 400, 180));
    }
  }

  // Change level when both are done
  if (player1.getFinished() && (player2.getFinished() || single_player)) {
    if (single_player) {
      asw::draw::sprite(
          results_singleplayer,
          asw::Vec2f((screenSize.x / 2) - 364, (screenSize.y / 2) - 200));
    } else {
      asw::draw::sprite(results, asw::Vec2f((screenSize.x / 2) - 364,
                                            (screenSize.y / 2) - 200));
    }

    hud_text(cooper, format_time(timer1),
             asw::Vec2f((screenSize.x / 2) - 60, (screenSize.y / 2) - 110));

    if (!single_player) {
      hud_text(cooper, format_time(timer2),
               asw::Vec2f((screenSize.x / 2) - 60, (screenSize.y / 2) - 55));

      if (timer1 < timer2) {
        hud_text(cooper, "1",
                 asw::Vec2f((screenSize.x / 2) - 175, (screenSize.y / 2) + 2));
        hud_text(cooper, format_time(timer2 - timer1),
                 asw::Vec2f((screenSize.x / 2) - 5, (screenSize.y / 2) + 2));
      } else if (timer1 > timer2) {
        hud_text(cooper, "2",
                 asw::Vec2f((screenSize.x / 2) - 175, (screenSize.y / 2) + 2));
        hud_text(cooper, format_time(timer1 - timer2),
                 asw::Vec2f((screenSize.x / 2) - 5, (screenSize.y / 2) + 2));
      }
    }
  }
}
