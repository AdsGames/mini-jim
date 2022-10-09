#include "Game.h"

#include <asw/util/KeyListener.h>
#include <asw/util/MouseListener.h>
#include <string>

#include "globals.h"
#include "utility/tools.h"

void Game::init() {
  // Player
  player1 = new Player(1);
  player2 = new Player(2);

  // Sets Font
  cooper = asw::load::font("assets/fonts/cooper.ttf", 24);

  // Load images
  countdownImage = asw::load::texture("assets/images/321go.png");
  darkness = asw::load::texture("assets/images/darkness.png");
  darkness_old = asw::load::texture("assets/images/darkness.png");

  spotlight = asw::load::texture("assets/images/spotlight.png");

  results = asw::load::texture("assets/images/gui/winscreen.png");
  results_singleplayer =
      asw::load::texture("assets/images/gui/winscreen_singleplayer.png");

  // Samples
  countdown = asw::load::sample("assets/sounds/countdown.wav");
  timeout = asw::load::sample("assets/sounds/timeout.wav");

  // Load music
  mainMusic = asw::load::sample("assets/sounds/music/BasicJimFull.ogg");

  tile_map = nullptr;

  // Init
  setup();
}

void Game::setup() {
  // Create map
  if (tile_map)
    delete tile_map;

  tile_map = new TileMap();

  std::string file_name = "assets/data/level_" + std::to_string(levelOn + 1);

  if (!tile_map->load(file_name)) {
    asw::util::abortOnError("Could not open level" + file_name);
  }

  auto screenSize = asw::display::getLogicalSize();

  if (single_player) {
    cam_1 = Camera(screenSize.x, screenSize.y, tile_map->getWidth(),
                   tile_map->getHeight());
    cam_2 = Camera(screenSize.x, screenSize.y, tile_map->getWidth(),
                   tile_map->getHeight());
  } else {
    cam_1 = Camera(screenSize.x, screenSize.y / 2, tile_map->getWidth(),
                   tile_map->getHeight());
    cam_2 = Camera(screenSize.x, screenSize.y / 2, tile_map->getWidth(),
                   tile_map->getHeight());
  }

  // Find spawn
  Tile* spawnTile = tile_map->find_tile_type(199, 1);

  if (spawnTile != nullptr) {
    player1->set_spawn(spawnTile->getX(), spawnTile->getY());
    player2->set_spawn(spawnTile->getX(), spawnTile->getY());
  }

  // Play music
  asw::sound::play(countdown, 255, 128, 0);
  asw::sound::play(mainMusic, 255, 128, 1);

  // Start game
  tm_begin.Start();
}

void Game::update() {
  // Camera follow
  cam_1.Follow(player1->getX(), player1->getY());
  cam_2.Follow(player2->getX(), player2->getY());

  // Starting countdown
  if (!tm_begin.IsRunning()) {
    // poll_joystick();

    // Stop from moving once done
    if (!player1->getFinished()) {
      player1->update(tile_map);
    }

    if (!player2->getFinished() && !single_player) {
      player2->update(tile_map);
    }
  }

  // Timers
  if (tm_begin.IsRunning() && tm_begin.GetElapsedTime<milliseconds>() > 1200) {
    tm_begin.Stop();
    tm_p1.Start();
    tm_p2.Start();
  }

  if (tm_p1.IsRunning() && player1->getFinished())
    tm_p1.Stop();

  if (tm_p2.IsRunning() && player2->getFinished())
    tm_p2.Stop();

  // Change level when both are done
  if (KeyListener::keyDown[SDL_SCANCODE_RETURN] && player1->getFinished() &&
      (player2->getFinished() || single_player)) {
    setNextState(StateEngine::STATE_MENU);
  }

  // Back to menu
  if (KeyListener::keyDown[SDL_SCANCODE_M]) {
    setNextState(StateEngine::STATE_MENU);
  }
}

void Game::draw() {
  auto screenSize = asw::display::getLogicalSize();

  // Draw tiles and characters

  if (single_player) {
    tile_map->draw(cam_1.GetX(), cam_1.GetY(), cam_1.GetWidth(),
                   cam_1.GetHeight());
    player1->draw(cam_1.GetX(), cam_1.GetY());
  }

  else {
    tile_map->draw(cam_1.GetX(), cam_1.GetY(), cam_1.GetWidth(),
                   cam_1.GetHeight(), 0, 0);
    player1->draw(cam_1.GetX(), cam_1.GetY());
    player2->draw(cam_1.GetX(), cam_1.GetY());

    tile_map->draw(cam_2.GetX(), cam_2.GetY(), cam_2.GetWidth(),
                   cam_2.GetHeight(), 0, screenSize.y / 2);
    player1->draw(cam_2.GetX(), cam_2.GetY() - screenSize.y / 2);
    player2->draw(cam_2.GetX(), cam_2.GetY() - screenSize.y / 2);
  }

  // Lighting
  // if (tile_map->hasLighting()) {
  //   // set_alpha_blender();
  //   asw::draw::sprite(darkness, darkness_old, 0, 0);

  //   // Get map area
  //   std::vector<Tile*> ranged_map = tile_map->get_tiles_in_range(
  //       cam_1.GetX() - spotlight->w,
  //       cam_1.GetX() + cam_1.GetWidth() + spotlight->w,
  //       cam_1.GetY() - spotlight->h,
  //       cam_1.GetY() + cam_1.GetHeight() + spotlight->w);

  //   for (auto t : ranged_map) {
  //     if (t->containsAttribute(light)) {
  //       asw::draw::stretchSprite(
  //           darkness, spotlight,
  //           t->getX() - cam_1.GetX() + t->getWidth() / 2 - t->getWidth() * 3,
  //           t->getY() - cam_1.GetY() + t->getHeight() / 2 - t->getHeight() *
  //           3, t->getWidth() * 6, t->getHeight() * 6);
  //     }
  //   }

  //   asw::draw::sprite(darkness, spotlight,
  //                     player1->getX() - cam_1.GetX() + 32 - (spotlight->w /
  //                     2), player1->getY() - cam_1.GetY() + 32 - (spotlight->h
  //                     / 2));
  //   asw::draw::sprite(screen1, darkness, 0, 0);
  // }

  // Frame
  asw::draw::primRectFill(0, 0, screenSize.x, 16,
                          asw::util::makeColor(0, 0, 0));
  asw::draw::primRectFill(0, 0, 16, screenSize.y,
                          asw::util::makeColor(0, 0, 0));
  asw::draw::primRectFill(screenSize.x - 16, 0, screenSize.x, screenSize.y,
                          asw::util::makeColor(0, 0, 0));
  asw::draw::primRectFill(0, screenSize.y - 16, screenSize.x, screenSize.y,
                          asw::util::makeColor(0, 0, 0));

  // Timers
  asw::draw::primRectFill(20, 20, 320, 90, asw::util::makeColor(0, 0, 0));

  if (!single_player) {
    asw::draw::primRectFill(20, (screenSize.y / 2) + 20, 320,
                            (screenSize.y / 2) + 90,
                            asw::util::makeColor(0, 0, 0));
  }

  // Draw timer to screen
  asw::draw::text(
      cooper,
      "Time: " + std::to_string(tm_p1.GetElapsedTime<milliseconds>() / 1000),
      40, 55, asw::util::makeColor(255, 255, 255, 255));
  asw::draw::text(cooper, "Deaths:" + std::to_string(player1->getDeathcount()),
                  40, 20, asw::util::makeColor(255, 255, 255, 255));

  if (!single_player) {
    asw::draw::text(
        cooper,
        "Time: " + std::to_string(tm_p2.GetElapsedTime<milliseconds>() / 1000),
        40, (screenSize.y / 2) + 20 + 35,
        asw::util::makeColor(255, 255, 255, 255));
    asw::draw::text(
        cooper, "Deaths:" + std::to_string(player2->getDeathcount()), 40,
        (screenSize.y / 2) + 20, asw::util::makeColor(255, 255, 255, 255));
  }

  // Starting countdown
  else {
    // Timer 3..2..1..GO!
    if (tm_begin.GetElapsedTime<milliseconds>() < 330) {
      asw::draw::stretchSpriteBlit(countdownImage, 0, 0, 14, 18,
                                   screenSize.x / 2 - 100,
                                   screenSize.y / 2 - 100, 140, 180);
    } else if (tm_begin.GetElapsedTime<milliseconds>() < 660) {
      asw::draw::stretchSpriteBlit(countdownImage, 19, 0, 14, 18,
                                   screenSize.x / 2 - 100,
                                   screenSize.y / 2 - 100, 140, 180);
    } else if (tm_begin.GetElapsedTime<milliseconds>() < 990) {
      asw::draw::stretchSpriteBlit(countdownImage, 39, 0, 14, 18,
                                   screenSize.x / 2 - 100,
                                   screenSize.y / 2 - 100, 140, 180);
    } else if (tm_begin.GetElapsedTime<milliseconds>() < 1200) {
      asw::draw::stretchSpriteBlit(countdownImage, 57, 0, 40, 18,
                                   screenSize.x / 2 - 200,
                                   screenSize.y / 2 - 100, 400, 180);
    }
  }

  // Change level when both are done
  if (player1->getFinished() && (player2->getFinished() || single_player)) {
    const float p1_time = tm_p1.GetElapsedTime<milliseconds>() / 1000;
    const float p2_time = tm_p1.GetElapsedTime<milliseconds>() / 1000;

    if (single_player)
      asw::draw::sprite(results_singleplayer, (screenSize.x / 2) - 364,
                        (screenSize.y / 2) - 200);
    else
      asw::draw::sprite(results, (screenSize.x / 2) - 364,
                        (screenSize.y / 2) - 200);

    asw::draw::text(cooper, std::to_string(p1_time), (screenSize.x / 2) - 60,
                    (screenSize.y / 2) - 110,
                    asw::util::makeColor(255, 255, 255, 255));

    if (!single_player) {
      asw::draw::text(cooper, std::to_string(p2_time), (screenSize.x / 2) - 60,
                      (screenSize.y / 2) - 55,
                      asw::util::makeColor(255, 255, 255, 255));

      if (p1_time < p2_time) {
        asw::draw::text(cooper, "1", (screenSize.x / 2) - 175,
                        (screenSize.y / 2) + 2,
                        asw::util::makeColor(255, 255, 255, 255));
        asw::draw::text(cooper, std::to_string(p2_time - p1_time),
                        (screenSize.x / 2) - 5, (screenSize.y / 2) + 2,
                        asw::util::makeColor(255, 255, 255, 255));
      } else if (p1_time > p2_time) {
        asw::draw::text(cooper, "2", (screenSize.x / 2) - 175,
                        (screenSize.y / 2) + 2,
                        asw::util::makeColor(255, 255, 255, 255));
        asw::draw::text(cooper, std::to_string(p1_time - p2_time),
                        (screenSize.x / 2) - 5, (screenSize.y / 2) + 2,
                        asw::util::makeColor(255, 255, 255, 255));
      }
    }
  }
}

void Game::cleanup() {
  delete player1;
  delete player2;
  delete tile_map;
}
