#include "Game.h"

#include <string>

#include "globals.h"
#include "utility/tools.h"

void Game::init() {
  // Player
  player1 = new Player(1);
  player2 = new Player(2);

  // Sets Font
  cooper = asw::assets::loadFont("assets/fonts/cooper.ttf", 24);

  // Load images
  countdownImage = asw::assets::loadTexture("assets/images/321go.png");

  results = asw::assets::loadTexture("assets/images/gui/winscreen.png");
  results_singleplayer =
      asw::assets::loadTexture("assets/images/gui/winscreen_singleplayer.png");

  // Samples
  countdown = asw::assets::loadSample("assets/sounds/countdown.wav");
  timeout = asw::assets::loadSample("assets/sounds/timeout.wav");

  // Load music
  mainMusic = asw::assets::loadSample("assets/sounds/music/BasicJimFull.ogg");

  tile_map = nullptr;

  // Init
  setup();
}

void Game::setup() {
  // Create map
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

  cam_1.SetSpeed(8.0F);
  cam_2.SetSpeed(8.0F);

  // Find spawn
  Tile* spawnTile = tile_map->find_tile_type(199, 1);

  if (spawnTile != nullptr) {
    player1->setSpawn(spawnTile->getX(), spawnTile->getY());
    player2->setSpawn(spawnTile->getX(), spawnTile->getY());
  }

  // Play music
  asw::sound::play(countdown, 255, 128, 0);
  asw::sound::play(mainMusic, 255, 128, 1);

  // Start game
  tm_begin.start();
}

void Game::update() {
  // Camera follow
  cam_1.Follow(player1->getX(), player1->getY());
  cam_2.Follow(player2->getX(), player2->getY());

  // Starting countdown
  if (!tm_begin.isRunning()) {
    // Stop from moving once done
    if (!player1->getFinished()) {
      player1->update(tile_map);
    }

    if (!player2->getFinished() && !single_player) {
      player2->update(tile_map);
    }
  }

  // Timers
  if (tm_begin.isRunning() &&
      tm_begin.getElapsedTime<std::chrono::milliseconds>() > 1200) {
    tm_begin.stop();
    tm_p1.start();
    tm_p2.start();
  }

  if (tm_p1.isRunning() && player1->getFinished())
    tm_p1.stop();

  if (tm_p2.isRunning() && player2->getFinished())
    tm_p2.stop();

  // Change level when both are done
  if (asw::input::keyboard.down[SDL_SCANCODE_RETURN] &&
      player1->getFinished() && (player2->getFinished() || single_player)) {
    setNextState(ProgramState::Menu);
  }

  // Back to menu
  if (asw::input::keyboard.down[SDL_SCANCODE_M]) {
    setNextState(ProgramState::Menu);
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
  if (tile_map->hasLighting()) {
    std::vector<SDL_Point> lightPointsP1;

    // Get map area
    std::vector<Tile*> rangeP1 = tile_map->get_tiles_in_range(
        cam_1.GetX(), cam_1.GetX() + cam_1.GetWidth(), cam_1.GetY(),
        cam_1.GetY() + cam_1.GetHeight());

    for (auto t : rangeP1) {
      if (t->containsAttribute(light)) {
        lightPointsP1.push_back(
            {t->getCenterX() - cam_1.GetX(), t->getCenterY() - cam_1.GetY()});
      }
    }

    lightPointsP1.push_back(
        {static_cast<int>(player1->getX() - cam_1.GetX() + 32),
         static_cast<int>(player1->getY() - cam_1.GetY() + 32)});

    lightLayer.draw(lightPointsP1);
  }

  // Frame
  asw::draw::rectFill(0, 0, screenSize.x, 16, asw::util::makeColor(0, 0, 0));
  asw::draw::rectFill(0, 0, 16, screenSize.y, asw::util::makeColor(0, 0, 0));
  asw::draw::rectFill(screenSize.x - 16, 0, screenSize.x, screenSize.y,
                      asw::util::makeColor(0, 0, 0));
  asw::draw::rectFill(0, screenSize.y - 16, screenSize.x, screenSize.y,
                      asw::util::makeColor(0, 0, 0));

  // Timers
  asw::draw::rectFill(20, 20, 320, 90, asw::util::makeColor(0, 0, 0));

  if (!single_player) {
    asw::draw::rectFill(20, (screenSize.y / 2) + 20, 320, 90,
                        asw::util::makeColor(0, 0, 0));
  }

  // Draw timer to screen
  asw::draw::text(
      cooper,
      "Time: " + std::to_string(
                     tm_p1.getElapsedTime<std::chrono::milliseconds>() / 1000),
      40, 55, asw::util::makeColor(255, 255, 255, 255));
  asw::draw::text(cooper, "Deaths:" + std::to_string(player1->getDeathcount()),
                  40, 20, asw::util::makeColor(255, 255, 255, 255));

  if (!single_player) {
    asw::draw::text(
        cooper,
        "Time: " +
            std::to_string(tm_p2.getElapsedTime<std::chrono::milliseconds>() /
                           1000),
        40, (screenSize.y / 2) + 20 + 35,
        asw::util::makeColor(255, 255, 255, 255));
    asw::draw::text(
        cooper, "Deaths:" + std::to_string(player2->getDeathcount()), 40,
        (screenSize.y / 2) + 20, asw::util::makeColor(255, 255, 255, 255));
  }

  // Starting countdown
  else {
    // Timer 3..2..1..GO!
    if (tm_begin.getElapsedTime<std::chrono::milliseconds>() < 330) {
      asw::draw::stretchSpriteBlit(countdownImage, 0, 0, 14, 18,
                                   screenSize.x / 2 - 100,
                                   screenSize.y / 2 - 100, 140, 180);
    } else if (tm_begin.getElapsedTime<std::chrono::milliseconds>() < 660) {
      asw::draw::stretchSpriteBlit(countdownImage, 19, 0, 14, 18,
                                   screenSize.x / 2 - 100,
                                   screenSize.y / 2 - 100, 140, 180);
    } else if (tm_begin.getElapsedTime<std::chrono::milliseconds>() < 990) {
      asw::draw::stretchSpriteBlit(countdownImage, 39, 0, 14, 18,
                                   screenSize.x / 2 - 100,
                                   screenSize.y / 2 - 100, 140, 180);
    } else if (tm_begin.getElapsedTime<std::chrono::milliseconds>() < 1200) {
      asw::draw::stretchSpriteBlit(countdownImage, 57, 0, 40, 18,
                                   screenSize.x / 2 - 200,
                                   screenSize.y / 2 - 100, 400, 180);
    }
  }

  // Change level when both are done
  if (player1->getFinished() && (player2->getFinished() || single_player)) {
    const float p1_time =
        tm_p1.getElapsedTime<std::chrono::milliseconds>() / 1000;
    const float p2_time =
        tm_p2.getElapsedTime<std::chrono::milliseconds>() / 1000;

    if (single_player) {
      asw::draw::sprite(results_singleplayer, (screenSize.x / 2) - 364,
                        (screenSize.y / 2) - 200);
    } else {
      asw::draw::sprite(results, (screenSize.x / 2) - 364,
                        (screenSize.y / 2) - 200);
    }

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
