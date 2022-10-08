#include "Game.h"

#include <string>

#include "globals.h"
#include "utility/KeyListener.h"
#include "utility/tools.h"

void Game::init(aar::Window* window) {
  // Player
  player1 = new Player(1);
  player2 = new Player(2);

  // Sets Font
  cooper = aar::load::font("assets/fonts/cooper.ttf", 24);

  // Load images
  countdownImage = aar::load::bitmap("assets/images/321go.png");
  darkness = aar::load::bitmap("assets/images/darkness.png");
  darkness_old = aar::load::bitmap("assets/images/darkness.png");

  spotlight = aar::load::bitmap("assets/images/spotlight.png");

  results = aar::load::bitmap("assets/images/gui/winscreen.png");
  results_singleplayer =
      aar::load::bitmap("assets/images/gui/winscreen_singleplayer.png");

  // Samples
  countdown = aar::load::sample("assets/sounds/countdown.wav");
  timeout = aar::load::sample("assets/sounds/timeout.wav");

  // Load music
  mainMusic = aar::load::sampleOgg("assets/sounds/music/BasicJimFull.ogg");

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
    aar::util::abortOnError("Could not open level" + file_name);
  }

  cam_1 =
      Camera(SCREEN_W, SCREEN_H, tile_map->getWidth(), tile_map->getHeight());
  cam_2 =
      Camera(SCREEN_W, SCREEN_H, tile_map->getWidth(), tile_map->getHeight());

  // Find spawn
  Tile* spawnTile = tile_map->find_tile_type(199, 1);

  if (spawnTile != nullptr) {
    player1->set_spawn(spawnTile->getX(), spawnTile->getY());
    player2->set_spawn(spawnTile->getX(), spawnTile->getY());
  }

  // Play music
  aar::sound::play(countdown, 255, 125, 1000, 0);
  aar::sound::play(mainMusic, 255, 125, 1000, 1);

  // Start game
  tm_begin.Start();
}

void Game::update(StateEngine& engine) {
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
  if (KeyListener::keyDown[SDLK_RETURN] && player1->getFinished() &&
      (player2->getFinished() || single_player)) {
    setNextState(engine, StateEngine::STATE_MENU);
  }

  // Back to menu
  if (KeyListener::keyDown[SDLK_m]) {
    setNextState(engine, StateEngine::STATE_MENU);
  }
}

void Game::draw(aar::Renderer* buffer) {
  // Black background (just in case)
  aar::draw::primRectFill(0, 0, NATIVE_SCREEN_W, NATIVE_SCREEN_H,
                          aar::util::makeColor(0, 0, 0, 255));

  // Draw tiles and characters
  tile_map->draw(cam_1.GetX(), cam_1.GetY());
  player1->draw(cam_1.GetX(), cam_1.GetY());

  // if (!single_player) {
  //   tile_map->draw(screen2, cam_2.GetX(), cam_2.GetY());
  //   player2->draw(screen1, cam_1.GetX(), cam_1.GetY());
  //   player1->draw(screen1, cam_1.GetX(), cam_1.GetY());
  //   player1->draw(screen2, cam_2.GetX(), cam_2.GetY());
  //   player2->draw(screen2, cam_2.GetX(), cam_2.GetY());
  // }

  // Lighting
  // if (tile_map->hasLighting()) {
  //   // set_alpha_blender();
  //   aar::draw::sprite(darkness, darkness_old, 0, 0);

  //   // Get map area
  //   std::vector<Tile*> ranged_map = tile_map->get_tiles_in_range(
  //       cam_1.GetX() - spotlight->w,
  //       cam_1.GetX() + cam_1.GetWidth() + spotlight->w,
  //       cam_1.GetY() - spotlight->h,
  //       cam_1.GetY() + cam_1.GetHeight() + spotlight->w);

  //   for (auto t : ranged_map) {
  //     if (t->containsAttribute(light)) {
  //       aar::draw::stretchSprite(
  //           darkness, spotlight,
  //           t->getX() - cam_1.GetX() + t->getWidth() / 2 - t->getWidth() * 3,
  //           t->getY() - cam_1.GetY() + t->getHeight() / 2 - t->getHeight() *
  //           3, t->getWidth() * 6, t->getHeight() * 6);
  //     }
  //   }

  //   aar::draw::sprite(darkness, spotlight,
  //                     player1->getX() - cam_1.GetX() + 32 - (spotlight->w /
  //                     2), player1->getY() - cam_1.GetY() + 32 - (spotlight->h
  //                     / 2));
  //   aar::draw::sprite(screen1, darkness, 0, 0);
  // }

  // Draw split screens
  // Screens
  // if (single_player) {
  //   aar::draw::stretchSprite(screen1, 0, 0, NATIVE_SCREEN_W,
  //   NATIVE_SCREEN_H);
  // } else {
  //   aar::draw::stretchSprite(screen1, 0, 0, NATIVE_SCREEN_W,
  //                            NATIVE_SCREEN_H / 2);
  //   aar::draw::stretchSprite(screen2, 0, NATIVE_SCREEN_H / 2,
  //   NATIVE_SCREEN_W,
  //                            NATIVE_SCREEN_H / 2);
  //   aar::draw::primRectFill(0, (NATIVE_SCREEN_H / 2) - 8, NATIVE_SCREEN_W,
  //                           (NATIVE_SCREEN_H / 2) + 8,
  //                           aar::util::makeColor(0, 0, 0, 255));
  // }

  // Frame
  aar::draw::primRectFill(0, 0, NATIVE_SCREEN_W, 16,
                          aar::util::makeColor(0, 0, 0, 255));
  aar::draw::primRectFill(0, 0, 16, NATIVE_SCREEN_H,
                          aar::util::makeColor(0, 0, 0, 255));
  aar::draw::primRectFill(NATIVE_SCREEN_W - 16, 0, NATIVE_SCREEN_W,
                          NATIVE_SCREEN_H, aar::util::makeColor(0, 0, 0, 255));
  aar::draw::primRectFill(0, NATIVE_SCREEN_H - 16, NATIVE_SCREEN_W,
                          NATIVE_SCREEN_H, aar::util::makeColor(0, 0, 0, 255));

  // Timers
  aar::draw::primRectFill(20, 20, 320, 90, aar::util::makeColor(0, 0, 0, 255));

  if (!single_player) {
    aar::draw::primRectFill(20, (NATIVE_SCREEN_H / 2) + 20, 320,
                            (NATIVE_SCREEN_H / 2) + 90,
                            aar::util::makeColor(0, 0, 0, 255));
  }

  // Draw timer to screen
  aar::draw::text(
      cooper,
      "Time: " + std::to_string(tm_p1.GetElapsedTime<milliseconds>() / 1000),
      40, 55, aar::util::makeColor(255, 255, 255, 255));
  aar::draw::text(cooper, "Deaths:" + std::to_string(player1->getDeathcount()),
                  40, 20, aar::util::makeColor(255, 255, 255, 255));

  if (!single_player) {
    aar::draw::text(
        cooper,
        "Time: " + std::to_string(tm_p2.GetElapsedTime<milliseconds>() / 1000),
        40, (NATIVE_SCREEN_H / 2) + 20 + 35,
        aar::util::makeColor(255, 255, 255, 255));
    aar::draw::text(
        cooper, "Deaths:" + std::to_string(player2->getDeathcount()), 40,
        (NATIVE_SCREEN_H / 2) + 20, aar::util::makeColor(255, 255, 255, 255));
  }

  // Starting countdown
  else {
    // Timer 3..2..1..GO!
    if (tm_begin.GetElapsedTime<milliseconds>() < 330) {
      aar::draw::stretchSpriteBlit(countdownImage, 0, 0, 14, 18,
                                   NATIVE_SCREEN_W / 2 - 100,
                                   NATIVE_SCREEN_H / 2 - 100, 140, 180);
    } else if (tm_begin.GetElapsedTime<milliseconds>() < 660) {
      aar::draw::stretchSpriteBlit(countdownImage, 19, 0, 14, 18,
                                   NATIVE_SCREEN_W / 2 - 100,
                                   NATIVE_SCREEN_H / 2 - 100, 140, 180);
    } else if (tm_begin.GetElapsedTime<milliseconds>() < 990) {
      aar::draw::stretchSpriteBlit(countdownImage, 39, 0, 14, 18,
                                   NATIVE_SCREEN_W / 2 - 100,
                                   NATIVE_SCREEN_H / 2 - 100, 140, 180);
    } else if (tm_begin.GetElapsedTime<milliseconds>() < 1200) {
      aar::draw::stretchSpriteBlit(countdownImage, 57, 0, 40, 18,
                                   NATIVE_SCREEN_W / 2 - 200,
                                   NATIVE_SCREEN_H / 2 - 100, 400, 180);
    }
  }

  // Change level when both are done
  if (player1->getFinished() && (player2->getFinished() || single_player)) {
    const float p1_time = tm_p1.GetElapsedTime<milliseconds>() / 1000;
    const float p2_time = tm_p1.GetElapsedTime<milliseconds>() / 1000;

    if (single_player)
      aar::draw::sprite(results_singleplayer, (NATIVE_SCREEN_W / 2) - 364,
                        (NATIVE_SCREEN_H / 2) - 200);
    else
      aar::draw::sprite(results, (NATIVE_SCREEN_W / 2) - 364,
                        (NATIVE_SCREEN_H / 2) - 200);

    aar::draw::text(cooper, std::to_string(p1_time), (NATIVE_SCREEN_W / 2) - 60,
                    (NATIVE_SCREEN_H / 2) - 110,
                    aar::util::makeColor(255, 255, 255, 255));

    if (!single_player) {
      aar::draw::text(cooper, std::to_string(p2_time),
                      (NATIVE_SCREEN_W / 2) - 60, (NATIVE_SCREEN_H / 2) - 55,
                      aar::util::makeColor(255, 255, 255, 255));

      if (p1_time < p2_time) {
        aar::draw::text(cooper, "1", (NATIVE_SCREEN_W / 2) - 175,
                        (NATIVE_SCREEN_H / 2) + 2,
                        aar::util::makeColor(255, 255, 255, 255));
        aar::draw::text(cooper, std::to_string(p2_time - p1_time),
                        (NATIVE_SCREEN_W / 2) - 5, (NATIVE_SCREEN_H / 2) + 2,
                        aar::util::makeColor(255, 255, 255, 255));
      } else if (p1_time > p2_time) {
        aar::draw::text(cooper, "2", (NATIVE_SCREEN_W / 2) - 175,
                        (NATIVE_SCREEN_H / 2) + 2,
                        aar::util::makeColor(255, 255, 255, 255));
        aar::draw::text(cooper, std::to_string(p1_time - p2_time),
                        (NATIVE_SCREEN_W / 2) - 5, (NATIVE_SCREEN_H / 2) + 2,
                        aar::util::makeColor(255, 255, 255, 255));
      }
    }
  }
}

Game::~Game() {
  // Destroy images
  aar::load::destroyBitmap(countdownImage);
  aar::load::destroyBitmap(results);
  aar::load::destroyBitmap(results_singleplayer);

  // Destroy fonts
  aar::load::destroyFont(cooper);

  // Destroy sounds
  aar::load::destroySample(countdown);
  aar::load::destroySample(timeout);
  aar::load::destroySample(mainMusic);

  // Fade out
  highcolor_fade_out(16);
}
