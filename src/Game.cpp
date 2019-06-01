#include "Game.h"

#include <string>

#include "globals.h"
#include "utility/tools.h"

Game::Game() {
  // Create screens
  if (single_player) {
    screen1 = create_bitmap (NATIVE_SCREEN_W, NATIVE_SCREEN_H);
    screen2 = create_bitmap (0, 0);
  }
  else {
    screen1 = create_bitmap (NATIVE_SCREEN_W, NATIVE_SCREEN_H / 2);
    screen2 = create_bitmap (NATIVE_SCREEN_W, NATIVE_SCREEN_H / 2);
  }

  lightingEnabled = (levelOn == 3);

  // Player
  player1.load_images (1);
  player1.load_sounds();
  player1.set_keys (KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ENTER_PAD, 0);

  player2.load_images (2);
  player2.load_sounds();
  player2.set_keys (KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, 1);

  // Build a color lookup table for lighting effects
  get_palette (pal);
  create_light_table (&light_table, pal, 0, 0, 0, nullptr);

  //Sets Font
  cooper = load_font_ex ("fonts/cooper_black.pcx");

  // Load images
  countdownImage = load_png_ex ("images/321go.png");
  darkness = load_png_ex ("images/darkness.png");
  darkness_old = load_png_ex ("images/darkness.png");

  spotlight = load_png_ex ("images/spotlight.png");

  results = load_png_ex ("images/gui/winscreen.png");
  results_singleplayer = load_png_ex ("images/gui/winscreen_singleplayer.png");

  // Samples
  countdown = load_sample_ex ("sounds/countdown.wav");
  timeout = load_sample_ex ("sounds/timeout.wav");

  // Load music
  mainMusic = load_ogg_ex("sounds/music/BasicJimFull.ogg");

  tile_map = nullptr;

  // Init
  init();
}

void Game::init() {
  // Create map
  if (tile_map)
    delete tile_map;

  tile_map = new TileMap ();

  std::string file_name = "data/level_" + std::to_string(levelOn + 1);
  if (!tile_map -> load (file_name.c_str()))
    abort_on_error(("Could not open level" + file_name).c_str());


  cam_1 = Camera(screen1 -> w, screen1 -> h, tile_map -> getWidth(), tile_map -> getHeight());
  cam_2 = Camera(screen2 -> w, screen2 -> h, tile_map -> getWidth(), tile_map -> getHeight());

  // Find spawn
  tile *spawnTile = tile_map -> find_tile_type(199, 1);
  if (spawnTile != nullptr) {
    player1.set_spawn (spawnTile -> getX(), spawnTile -> getY());
    player2.set_spawn (spawnTile -> getX(), spawnTile -> getY());
  }

  // Play music
  play_sample (countdown, 255, 125, 1000, 0);
  play_sample (mainMusic, 255, 125, 1000, 1);

  // Start game
  tm_begin.Start();
}

void Game::update(StateEngine *engine) {
  // Camera follow
  cam_1.Follow(player1.getX(), player1.getY());
  cam_2.Follow(player2.getX(), player2.getY());

  // Starting countdown
  if (!tm_begin.IsRunning()) {
    poll_joystick();

    // Stop from moving once done
    if (!player1.getFinished()) {
      player1.update (tile_map);
    }
    if (!player2.getFinished() && !single_player) {
      player2.update (tile_map);
    }
  }

  // Timers
  if (tm_begin.IsRunning() && tm_begin.GetElapsedTime<milliseconds>() > 1200) {
    tm_begin.Stop();
    tm_p1.Start();
    tm_p2.Start();
  }
  if (tm_p1.IsRunning() && player1.getFinished())
    tm_p1.Stop();
  if (tm_p2.IsRunning() && player2.getFinished())
    tm_p2.Stop();

  // Change level when both are done
  if (key[KEY_ENTER] && player1.getFinished() && (player2.getFinished() || single_player)) {
    setNextState (engine, StateEngine::STATE_MENU);
  }

  // Back to menu
  if (key[KEY_M])
    setNextState (engine, StateEngine::STATE_MENU);
}

void Game::draw(BITMAP *buffer) {
  // Black background (just in case)
  rectfill (buffer, 0, 0, NATIVE_SCREEN_W, NATIVE_SCREEN_H, 0x000000);

  // Draw tiles and characters
  tile_map -> draw (screen1, cam_1.GetX(), cam_1.GetY());
  player1.draw (screen1, cam_1.GetX(), cam_1.GetY());

  if (!single_player) {
    tile_map -> draw (screen2, cam_2.GetX(), cam_2.GetY());
    player2.draw (screen1, cam_1.GetX(), cam_1.GetY());
    player1.draw (screen1, cam_1.GetX(), cam_1.GetY());
    player1.draw (screen2, cam_2.GetX(), cam_2.GetY());
    player2.draw (screen2, cam_2.GetX(), cam_2.GetY());
  }

  // Lighting
  if (lightingEnabled) {
    set_alpha_blender();

    // Player 1
    std::vector<tile*> ranged_map = tile_map -> get_tiles_in_range(cam_1.GetX(), cam_1.GetX() + cam_1.GetWidth(), cam_1.GetY(), cam_1.GetY() + cam_1.GetHeight());

    //Check for collision
    draw_sprite (darkness, darkness_old, 0, 0);
    for (auto t : ranged_map) {
      if (t -> containsAttribute (light)) {
        stretch_sprite (darkness, spotlight,
                        t -> getX() - cam_1.GetX() + t -> getWidth()  / 2 - t -> getWidth() * 3,
                        t -> getY() - cam_1.GetY() + t -> getHeight() / 2 - t -> getHeight() * 3,
                        t -> getWidth() * 6, t -> getHeight() * 6);
      }
    }
    draw_sprite (darkness, spotlight, player1.getX() - cam_1.GetX() + 32 - (spotlight->w / 2), player1.getY() - cam_1.GetY() + 32 - (spotlight->h / 2));
    draw_trans_sprite (screen1, darkness, 0, 0);
  }

  // Draw split screens
  // Screens
  if (single_player) {
    stretch_sprite (buffer, screen1, 0, 0, NATIVE_SCREEN_W, NATIVE_SCREEN_H);
  }
  else {
    stretch_sprite (buffer, screen1, 0, 0, NATIVE_SCREEN_W, NATIVE_SCREEN_H / 2);
    stretch_sprite (buffer, screen2, 0, NATIVE_SCREEN_H / 2, NATIVE_SCREEN_W, NATIVE_SCREEN_H / 2);
    rectfill (buffer, 0, (NATIVE_SCREEN_H / 2) - 8, NATIVE_SCREEN_W, (NATIVE_SCREEN_H / 2) + 8, makecol (0, 0, 0));
  }

  // Frame
  rectfill (buffer, 0, 0, NATIVE_SCREEN_W, 16, makecol (0, 0, 0));
  rectfill (buffer, 0, 0, 16, NATIVE_SCREEN_H, makecol (0, 0, 0));
  rectfill (buffer, NATIVE_SCREEN_W - 16, 0, NATIVE_SCREEN_W, NATIVE_SCREEN_H, makecol (0, 0, 0));
  rectfill (buffer, 0, NATIVE_SCREEN_H - 16, NATIVE_SCREEN_W, NATIVE_SCREEN_H, makecol (0, 0, 0));

  // Timers
  rectfill (buffer, 20, 20, 320, 90, makecol (0, 0, 0));

  if (!single_player)
    rectfill (buffer, 20, (NATIVE_SCREEN_H / 2) + 20, 320, (NATIVE_SCREEN_H / 2) + 90, makecol (0, 0, 0));

  // Draw timer to screen
  textprintf_ex (buffer, cooper, 40, 55, makecol (255, 255, 255), -1, "Time: %.1f", tm_p1.GetElapsedTime<milliseconds>() / 1000);
  textprintf_ex (buffer, cooper, 40, 20, makecol (255, 255, 255), -1, "Deaths:%i", player1.getDeathcount());

  if (!single_player) {
    textprintf_ex (buffer, cooper, 40, (NATIVE_SCREEN_H / 2) + 20 + 35, makecol (255, 255, 255), -1, "Time: %.1f", tm_p2.GetElapsedTime<milliseconds>() / 1000);
    textprintf_ex (buffer, cooper, 40, (NATIVE_SCREEN_H / 2) + 20, makecol (255, 255, 255), -1, "Deaths:%i", player2.getDeathcount());
  }

  // Starting countdown
  else {
    // Timer 3..2..1..GO!
    if (tm_begin.GetElapsedTime<milliseconds>() < 330)
      masked_stretch_blit (countdownImage, buffer, 0, 0, 14, 18, NATIVE_SCREEN_W / 2 - 100, NATIVE_SCREEN_H / 2 - 100, 140, 180);
    else if (tm_begin.GetElapsedTime<milliseconds>() < 660)
      masked_stretch_blit (countdownImage, buffer, 19, 0, 14, 18, NATIVE_SCREEN_W / 2 - 100, NATIVE_SCREEN_H / 2 - 100, 140, 180);
    else if (tm_begin.GetElapsedTime<milliseconds>() < 990)
      masked_stretch_blit (countdownImage, buffer, 39, 0, 14, 18, NATIVE_SCREEN_W / 2 - 100, NATIVE_SCREEN_H / 2 - 100, 140, 180);
    else if (tm_begin.GetElapsedTime<milliseconds>() < 1200)
      masked_stretch_blit (countdownImage, buffer, 57, 0, 40, 18, NATIVE_SCREEN_W / 2 - 200, NATIVE_SCREEN_H / 2 - 100, 400, 180);
  }

  // Change level when both are done
  if (player1.getFinished() && (player2.getFinished() || single_player)) {
    const float p1_time = tm_p1.GetElapsedTime<milliseconds>() / 1000;
    const float p2_time = tm_p1.GetElapsedTime<milliseconds>() / 1000;

    set_alpha_blender();

    if (single_player)
      draw_trans_sprite (buffer, results_singleplayer, (NATIVE_SCREEN_W / 2) - 364, (NATIVE_SCREEN_H / 2) - 200);
    else
      draw_trans_sprite (buffer, results, (NATIVE_SCREEN_W / 2) - 364, (NATIVE_SCREEN_H / 2) - 200);

    textprintf_ex (buffer, cooper, (NATIVE_SCREEN_W / 2) - 60, (NATIVE_SCREEN_H / 2) - 110, makecol (255, 255, 255), -1, "%.1f", p1_time);

    if (!single_player) {
      textprintf_ex (buffer, cooper, (NATIVE_SCREEN_W / 2) - 60, (NATIVE_SCREEN_H / 2) - 55, makecol (255, 255, 255), -1, "%.1f", p2_time);

      if (p1_time < p2_time) {
        textprintf_ex (buffer, cooper, (NATIVE_SCREEN_W / 2) - 175, (NATIVE_SCREEN_H / 2) + 2, makecol (255, 255, 255), -1, "1");
        textprintf_ex (buffer, cooper, (NATIVE_SCREEN_W / 2) - 5, (NATIVE_SCREEN_H / 2) + 2, makecol (255, 255, 255), -1, "%.1f", p2_time - p1_time);
      }
      else if (p1_time > p2_time) {
        textprintf_ex (buffer, cooper, (NATIVE_SCREEN_W / 2) - 175, (NATIVE_SCREEN_H / 2) + 2, makecol (255, 255, 255), -1, "2");
        textprintf_ex (buffer, cooper, (NATIVE_SCREEN_W / 2) - 5, (NATIVE_SCREEN_H / 2) + 2, makecol (255, 255, 255), -1, "%.1f", p1_time - p2_time);
      }
    }
  }
}

Game::~Game() {
  // Destroy images
  destroy_bitmap (screen1);
  destroy_bitmap (screen2);
  destroy_bitmap (countdownImage);
  destroy_bitmap (results);
  destroy_bitmap (results_singleplayer);

  // Destroy fonts
  destroy_font (cooper);

  // Destroy sounds
  destroy_sample (countdown);
  destroy_sample (timeout);
  destroy_sample (mainMusic);

  // Fade out
  highcolor_fade_out (16);
}
