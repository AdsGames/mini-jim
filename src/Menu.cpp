#include "Menu.h"

#include "utility/KeyListener.h"
#include "utility/MouseListener.h"

// Create menu
Menu::Menu() {

  // Create buffer image
  buffer = create_bitmap (SCREEN_W, SCREEN_H);

  // Load images
  menu = load_png_ex("images/gui/menu.png");
  menuselect = load_png_ex("images/gui/menuSelector.png");
  help = load_png_ex("images/gui/help.png");
  cursor = load_png_ex ("images/gui/cursor1.png");
  levelSelectNumber = load_png_ex("images/gui/levelSelectNumber.png");
  copyright = load_png_ex("images/gui/copyright.png");
  credits = load_png_ex("images/gui/credits.png");

  //Load sound effects
  click = load_sample_ex("sounds/click.wav");
  intro = load_sample_ex("sounds/intro.wav");

  //Sets Font
  font = load_font_ex("fonts/arial_black.pcx");

  // Load music
  music = load_ogg_ex("sounds/music/MiniJim.ogg");

  // Allow transparency
  set_alpha_blender();

  //Variables
  selected_button = 0;
  move_to_button = 0;

  // Create map for live background
  tile_map = new tileMap ("data/level_1");
  //tile_map -> y = 4000;

  // Buttons
  btn_start = Button(60, 630);
  btn_start_mp = Button(60, 690);
  btn_edit = Button(60, 750);
  btn_help = Button(60, 810);
  btn_exit = Button(60, 870);
  btn_left = Button(SCREEN_W - 180, 80);
  btn_right = Button(SCREEN_W - 80, 80);

  btn_start.SetImages("images/gui/button_start.png", "images/gui/button_start.png");
  btn_start_mp.SetImages("images/gui/button_start_mp.png", "images/gui/button_start_mp.png");
  btn_edit.SetImages("images/gui/button_edit.png", "images/gui/button_edit.png");
  btn_help.SetImages("images/gui/button_help.png", "images/gui/button_help.png");
  btn_exit.SetImages("images/gui/button_quit.png", "images/gui/button_quit.png");
  btn_left.SetImages("images/gui/button_left.png", "images/gui/button_left_hover.png");
  btn_right.SetImages("images/gui/button_right.png", "images/gui/button_right_hover.png");

  target_selector_y = btn_start.GetY();
  selector_y = btn_start.GetY();

  // Set background scroll dir
  scrollDirection = 1;

  play_sample (music, 255, 125, 1000, 1);
  play_sample (intro, 255, 128, 1000, 0);

  levelOn = 0;
}

void Menu::update() {
  // Move around live background
  tile_map -> x += scrollDirection * 2;
  if (tile_map -> x >= tile_map -> width * 32 || tile_map -> x <= 0)
    scrollDirection *= -1;

  // Move selector
  if (selector_y != target_selector_y)
    selector_y += (target_selector_y - selector_y) / 3.0f;

  // Change level
  if (KeyListener::keyPressed[KEY_A] || KeyListener::keyPressed[KEY_LEFT] ||
     (MouseListener::mouse_pressed & 1 && selected_button == 5)) {
    play_sample (click, 255, 125, 1000, 0);
    levelOn --;
    levelOn = levelOn % 3 + (levelOn < 0) * 3;
    tile_map -> load (std::string("data/level_" + std::to_string(levelOn + 1)).c_str());
  }
  if (KeyListener::keyPressed[KEY_D] || KeyListener::keyPressed[KEY_RIGHT] ||
     (MouseListener::mouse_pressed & 1 && selected_button == 6)) {
    play_sample (click, 255, 125, 1000, 0);
    levelOn ++;
    levelOn = levelOn % 3 + (levelOn < 0) * 3;
    tile_map -> load (std::string("data/level_" + std::to_string(levelOn + 1)).c_str());
  }

  // Change menu
  if (KeyListener::keyPressed[KEY_W] || KeyListener::keyPressed[KEY_UP]) {
    move_to_button = (move_to_button - 1 < 0) ? 4 : move_to_button - 1;
  }

  if (KeyListener::keyPressed[KEY_S] || KeyListener::keyPressed[KEY_DOWN]) {
    move_to_button = (move_to_button + 1) % 5;
  }

  if (btn_start.Hover())
    move_to_button = 0;
  else if (btn_start_mp.Hover())
    move_to_button = 1;
  else if (btn_edit.Hover())
    move_to_button = 2;
  else if (btn_help.Hover())
    move_to_button = 3;
  else if (btn_exit.Hover())
    move_to_button = 4;
  else if (btn_left.Hover())
    move_to_button = 5;
  else if (btn_right.Hover())
    move_to_button = 6;

  // Select button
  if (mouse_b & 1 || KeyListener::keyPressed[KEY_ENTER]) {
    switch (selected_button) {
      case 0:
        single_player = true;
        set_next_state (STATE_GAME);
        break;
      case 1:
        single_player = false;
        set_next_state (STATE_GAME);
        break;
      case 2:
        set_next_state (STATE_EDIT);
        break;
      case 4:
        set_next_state (STATE_EXIT);
        break;
      default:
        break;
    }
  }

  // Move to selection
  if (selected_button != move_to_button) {
    play_sample (click, 255, 125, 1000, 0);
    selected_button = move_to_button;

    switch (selected_button) {
      case 0:
        target_selector_y = btn_start.GetY();
        break;
      case 1:
        target_selector_y = btn_start_mp.GetY();
        break;
      case 2:
        target_selector_y = btn_edit.GetY();
        break;
      case 3:
        target_selector_y = btn_help.GetY();
        break;
      case 4:
        target_selector_y = btn_exit.GetY();
        break;
      default:
        break;
    }
  }
}

void Menu::draw() {
  // Draw background to screen
  rectfill (buffer, 0, 0, SCREEN_W, SCREEN_H, makecol (255, 255, 255));

  // Draw live background
  tile_map -> draw_map (buffer);

  // Overlay
  draw_trans_sprite (buffer, credits, 0, 0);
  draw_trans_sprite (buffer, menu, 40, 480);
  draw_trans_sprite (buffer, menuselect, 50, selector_y);

  // Buttons
  btn_start.Draw(buffer);
  btn_start_mp.Draw(buffer);
  btn_edit.Draw(buffer);
  btn_help.Draw(buffer);
  btn_exit.Draw(buffer);
  btn_left.Draw(buffer);
  btn_right.Draw(buffer);

  // Level selection
  draw_trans_sprite (buffer, levelSelectNumber, SCREEN_W - 160, 80);
  textprintf_centre_ex (buffer, font, SCREEN_W - 113, 76, 0x000000, -1, "%i", levelOn + 1);

  // Cursor
  draw_sprite (buffer, cursor, mouse_x, mouse_y);

  // Help menu
  if (selected_button == 3)
    draw_trans_sprite (buffer, help, 0, 0);

  draw_trans_sprite (buffer, copyright, SCREEN_W - 350, SCREEN_H - 40);
  draw_sprite (buffer, cursor, mouse_x, mouse_y);

  // Draw buffer
  stretch_sprite (screen, buffer, 0, 0, SCREEN_W, SCREEN_H);
}

Menu::~Menu() {
  // Destory Bitmaps
  destroy_bitmap (buffer);
  destroy_bitmap (levelSelectNumber);
  destroy_bitmap (cursor);
  destroy_bitmap (menuselect);
  destroy_bitmap (menu);
  destroy_bitmap (help);
  destroy_bitmap (copyright);
  destroy_bitmap (credits);

  // Destory Samples
  destroy_sample (click);
  destroy_sample (intro);
  destroy_sample (music);

  // Destory background
  delete tile_map;

  // Fade out
  highcolor_fade_out (16);
}
