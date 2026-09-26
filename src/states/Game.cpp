#include "./Game.h"

#include <string>

#include "../globals.h"

void Game::init() {
  // Player
  player1 = Player(1);
  player2 = Player(2);

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

  if (single_player) {
    cam_1 = Camera(screenSize.x, screenSize.y, tile_map.getWidth(),
                   tile_map.getHeight());
    cam_2 = Camera(screenSize.x, screenSize.y, tile_map.getWidth(),
                   tile_map.getHeight());
  } else {
    cam_1 = Camera(screenSize.x, screenSize.y / 2, tile_map.getWidth(),
                   tile_map.getHeight());
    cam_2 = Camera(screenSize.x, screenSize.y / 2, tile_map.getWidth(),
                   tile_map.getHeight());
  }

  cam_1.setSpeed(8.0F);
  cam_2.setSpeed(8.0F);

  // Find spawn
  Tile* spawnTile = tile_map.find_tile_type(199, 1);

  if (spawnTile != nullptr) {
    player1.setSpawn(spawnTile->getTransform().position);
    player2.setSpawn(spawnTile->getTransform().position);
  }

  // Play music
  asw::sound::play(countdown);
  asw::sound::play_music(mainMusic);

  // Start game
  tm_begin.start();
}

void Game::update(float dt) {
  // asw passes seconds; game logic is tuned in milliseconds
  dt *= 1000.0F;

  // Camera follow
  cam_1.follow(player1.getTransform().position, dt);
  cam_2.follow(player2.getTransform().position, dt);

  // Tile
  tile_map.update(dt);

  // Starting countdown
  if (!tm_begin.isRunning()) {
    // Stop from moving once done
    if (!player1.getFinished()) {
      player1.update(tile_map, dt);
    }

    if (!player2.getFinished() && !single_player) {
      player2.update(tile_map, dt);
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
  if (asw::input::get_key_down(asw::input::Key::Return) &&
      player1.getFinished() && (player2.getFinished() || single_player)) {
    manager.set_next_scene(ProgramState::Menu);
  }

  // Back to menu
  if (asw::input::get_key_down(asw::input::Key::Escape)) {
    manager.set_next_scene(ProgramState::Menu);
  }
}

void Game::draw() {
  auto screenSize = asw::display::get_logical_size();

  // Draw tiles and characters
  if (single_player) {
    tile_map.draw(cam_1.getViewport(), 0, 0, 1);
    player1.draw(cam_1.getViewport().position);
    tile_map.drawShadows(cam_1.getViewport(), 0, 0);
    tile_map.draw(cam_1.getViewport(), 0, 0, 2);
    tile_map.drawLights(cam_1.getViewport(), 0, 0);
  } else {
    // Clip to remove interference
    SDL_Rect clip;
    clip.x = 0;
    clip.w = screenSize.x;

    // Top
    clip.y = 0;
    clip.h = screenSize.y / 2;

    SDL_SetRenderClipRect(asw::display::get_renderer(), &clip);
    tile_map.draw(cam_1.getViewport(), 0, 0, 1);

    player1.draw(cam_1.getViewport().position);
    player2.draw(cam_1.getViewport().position);

    tile_map.drawShadows(cam_1.getViewport(), 0, 0);
    tile_map.draw(cam_1.getViewport(), 0, 0, 2);
    tile_map.drawLights(cam_1.getViewport(), 0, 0);

    // Bottom
    clip.y = screenSize.y / 2;
    clip.h = screenSize.y / 2;

    SDL_SetRenderClipRect(asw::display::get_renderer(), &clip);
    tile_map.draw(cam_2.getViewport(), 0, screenSize.y / 2, 1);

    player1.draw(cam_2.getViewport().position +
                 asw::Vec2<float>(0, -screenSize.y / 2));
    player2.draw(cam_2.getViewport().position +
                 asw::Vec2<float>(0, -screenSize.y / 2));

    tile_map.drawShadows(cam_2.getViewport(), 0, screenSize.y / 2);
    tile_map.draw(cam_2.getViewport(), 0, screenSize.y / 2, 2);
    tile_map.drawLights(cam_2.getViewport(), 0, screenSize.y / 2);

    SDL_SetRenderClipRect(asw::display::get_renderer(), nullptr);
  }

  // Frame
  asw::draw::rect_fill(asw::Quad<float>(0, 0, screenSize.x, 16),
                      asw::Color(0, 0, 0));
  asw::draw::rect_fill(asw::Quad<float>(0, 0, 16, screenSize.y),
                      asw::Color(0, 0, 0));
  asw::draw::rect_fill(
      asw::Quad<float>(screenSize.x - 16, 0, screenSize.x, screenSize.y),
      asw::Color(0, 0, 0));
  asw::draw::rect_fill(
      asw::Quad<float>(0, screenSize.y - 16, screenSize.x, screenSize.y),
      asw::Color(0, 0, 0));

  // Timers
  asw::draw::rect_fill(asw::Quad<float>(20, 20, 320, 90),
                      asw::Color(0, 0, 0));

  if (!single_player) {
    asw::draw::rect_fill(asw::Quad<float>(20, (screenSize.y / 2) + 20, 320, 90),
                        asw::Color(0, 0, 0));
  }

  // Draw timer to screen
  const auto timer1 =
      std::roundf(tm_p1.getElapsedTime<std::chrono::milliseconds>() / 100) / 10;
  const auto timer2 =
      std::roundf(tm_p2.getElapsedTime<std::chrono::milliseconds>() / 100) / 10;

  asw::draw::text(cooper, "Time: " + std::to_string(timer1),
                  asw::Vec2<float>(40, 55),
                  asw::Color(255, 255, 255, 255));

  asw::draw::text(cooper, "Deaths:" + std::to_string(player1.getDeathcount()),
                  asw::Vec2<float>(40, 20),
                  asw::Color(255, 255, 255, 255));

  if (!single_player) {
    asw::draw::text(cooper, "Time: " + std::to_string(timer2),
                    asw::Vec2<float>(40, (screenSize.y / 2) + 20 + 35),
                    asw::Color(255, 255, 255, 255));

    asw::draw::text(cooper, "Deaths:" + std::to_string(player2.getDeathcount()),
                    asw::Vec2<float>(40, (screenSize.y / 2) + 20),
                    asw::Color(255, 255, 255, 255));
  }

  // Starting countdown
  else {
    // Timer 3..2..1..GO!
    if (tm_begin.getElapsedTime<std::chrono::milliseconds>() < 330) {
      asw::draw::stretch_sprite_blit(
          countdownImage, asw::Quad<float>(0, 0, 14, 18),
          asw::Quad<float>(screenSize.x / 2 - 100, screenSize.y / 2 - 100, 140,
                           180));
    } else if (tm_begin.getElapsedTime<std::chrono::milliseconds>() < 660) {
      asw::draw::stretch_sprite_blit(
          countdownImage, asw::Quad<float>(19, 0, 14, 18),
          asw::Quad<float>(screenSize.x / 2 - 100, screenSize.y / 2 - 100, 140,
                           180));
    } else if (tm_begin.getElapsedTime<std::chrono::milliseconds>() < 990) {
      asw::draw::stretch_sprite_blit(
          countdownImage, asw::Quad<float>(39, 0, 14, 18),
          asw::Quad<float>(screenSize.x / 2 - 100, screenSize.y / 2 - 100, 140,
                           180));
    } else if (tm_begin.getElapsedTime<std::chrono::milliseconds>() < 1200) {
      asw::draw::stretch_sprite_blit(
          countdownImage, asw::Quad<float>(57, 0, 40, 18),
          asw::Quad<float>(screenSize.x / 2 - 200, screenSize.y / 2 - 100, 400,
                           180));
    }
  }

  // Change level when both are done
  if (player1.getFinished() && (player2.getFinished() || single_player)) {
    if (single_player) {
      asw::draw::sprite(
          results_singleplayer,
          asw::Vec2<float>((screenSize.x / 2) - 364, (screenSize.y / 2) - 200));
    } else {
      asw::draw::sprite(results, asw::Vec2<float>((screenSize.x / 2) - 364,
                                                  (screenSize.y / 2) - 200));
    }

    asw::draw::text(
        cooper, std::to_string(timer1),
        asw::Vec2<float>((screenSize.x / 2) - 60, (screenSize.y / 2) - 110),
        asw::Color(255, 255, 255, 255));

    if (!single_player) {
      asw::draw::text(
          cooper, std::to_string(timer2),
          asw::Vec2<float>((screenSize.x / 2) - 60, (screenSize.y / 2) - 55),
          asw::Color(255, 255, 255, 255));

      if (timer1 < timer2) {
        asw::draw::text(
            cooper, "1",
            asw::Vec2<float>((screenSize.x / 2) - 175, (screenSize.y / 2) + 2),
            asw::Color(255, 255, 255, 255));
        asw::draw::text(
            cooper, std::to_string(timer2 - timer1),
            asw::Vec2<float>((screenSize.x / 2) - 5, (screenSize.y / 2) + 2),
            asw::Color(255, 255, 255, 255));
      } else if (timer1 > timer2) {
        asw::draw::text(
            cooper, "2",
            asw::Vec2<float>((screenSize.x / 2) - 175, (screenSize.y / 2) + 2),
            asw::Color(255, 255, 255, 255));
        asw::draw::text(
            cooper, std::to_string(timer1 - timer2),
            asw::Vec2<float>((screenSize.x / 2) - 5, (screenSize.y / 2) + 2),
            asw::Color(255, 255, 255, 255));
      }
    }
  }
}
