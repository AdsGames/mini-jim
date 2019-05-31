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

  //Load sound
  click = load_sample_ex("sounds/click.wav");
  intro = load_sample_ex("sounds/intro.wav");
  music = load_ogg_ex("sounds/music/MiniJim.ogg");

  //Sets Font
  font = load_font_ex("fonts/arial_black.pcx");

  // Allow transparency
  set_alpha_blender();

  // Create map for live background
  levelOn = 0;
  tile_map = new TileMap();
  change_level(0);

  // Buttons
  buttons[BUTTON_START] = Button(60, 630);
  buttons[BUTTON_START_MP] = Button(60, 690);
  buttons[BUTTON_EDIT] = Button(60, 750);
  buttons[BUTTON_HELP] = Button(60, 810);
  buttons[BUTTON_EXIT] = Button(60, 870);
  buttons[BUTTON_LEFT] = Button(SCREEN_W - 180, 80);
  buttons[BUTTON_RIGHT] = Button(SCREEN_W - 80, 80);

  buttons[BUTTON_START].SetImages("images/gui/button_start.png", "images/gui/button_start.png");
  buttons[BUTTON_START_MP].SetImages("images/gui/button_start_mp.png", "images/gui/button_start_mp.png");
  buttons[BUTTON_EDIT].SetImages("images/gui/button_edit.png", "images/gui/button_edit.png");
  buttons[BUTTON_HELP].SetImages("images/gui/button_help.png", "images/gui/button_help.png");
  buttons[BUTTON_EXIT].SetImages("images/gui/button_quit.png", "images/gui/button_quit.png");
  buttons[BUTTON_LEFT].SetImages("images/gui/button_left.png", "images/gui/button_left_hover.png");
  buttons[BUTTON_RIGHT].SetImages("images/gui/button_right.png", "images/gui/button_right_hover.png");

  //Variables
  selected_button = 0;
  move_to_button = 0;
  target_selector_y = buttons[BUTTON_START].GetY();
  selector_y = target_selector_y;

  play_sample (music, 255, 125, 1000, 1);
  play_sample (intro, 255, 128, 1000, 0);
}

bool Menu::button_hover() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    if (buttons[i].Hover()) {
      return true;
    }
  }
  return false;
}

void Menu::change_level(int level) {
  levelOn = (levelOn + level) < 0 ? 2 : (levelOn + level) % 3;
  tile_map -> load (std::string("data/level_" + std::to_string(levelOn + 1)).c_str());

  scroll_x = random(SCREEN_W, tile_map -> width * 64 - SCREEN_W);
  scroll_dir_x = random (0, 1) ? -3 : 3;
  scroll_y = random(SCREEN_H, tile_map -> height * 64 - SCREEN_H);
  scroll_dir_y = random (0, 1) ? -3 : 3;

  play_sample (click, 255, 125, 1000, 0);

  cam = Camera(SCREEN_W, SCREEN_H, tile_map -> width * 64, tile_map -> height * 64);
  cam.SetSpeed(1);
}

void Menu::update() {
  // Move around live background
  if (scroll_x + SCREEN_W / 2 >= tile_map -> width * 64 || scroll_x <= SCREEN_W / 2)
    scroll_dir_x *= -1;
  if (scroll_y + SCREEN_H / 2 >= tile_map -> height * 64 || scroll_y <= SCREEN_H / 2)
    scroll_dir_y *= -1;
  scroll_x += scroll_dir_x;
  scroll_y += scroll_dir_y;

  cam.Follow(scroll_x, scroll_y);

  // Move selector
  selector_y += (target_selector_y - selector_y) / 3.0f;

  // Change menu
  if (KeyListener::keyPressed[KEY_W] || KeyListener::keyPressed[KEY_UP])
    move_to_button = (move_to_button - 1 < 0) ? 4 : move_to_button - 1;

  if (KeyListener::keyPressed[KEY_S] || KeyListener::keyPressed[KEY_DOWN])
    move_to_button = (move_to_button + 1) % 5;

  // Check hovers
  for (int i = 0; i < NUM_BUTTONS; i++) {
    if (buttons[i].Hover()) {
      move_to_button = i;
    }
  }

  // Select button
  if ((mouse_b & 1 && button_hover()) || KeyListener::keyPressed[KEY_ENTER]) {
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

  // Change level
  if (KeyListener::keyPressed[KEY_A] || KeyListener::keyPressed[KEY_LEFT] ||
     (MouseListener::mouse_pressed & 1 && selected_button == 5)) {
    change_level(-1);
  }
  if (KeyListener::keyPressed[KEY_D] || KeyListener::keyPressed[KEY_RIGHT] ||
     (MouseListener::mouse_pressed & 1 && selected_button == 6)) {
    change_level(1);
  }

  // Move to selection
  if (selected_button != move_to_button) {
    play_sample (click, 255, 125, 1000, 0);
    selected_button = move_to_button;
    target_selector_y = buttons[selected_button].GetY();
  }
}

void Menu::draw() {
  // Draw background to screen
  rectfill (buffer, 0, 0, SCREEN_W, SCREEN_H, makecol (255, 255, 255));

  // Draw live background
  tile_map -> draw (buffer, cam.GetX(), cam.GetY());

  // Overlay
  draw_trans_sprite (buffer, credits, 0, 0);
  draw_trans_sprite (buffer, menu, 40, 480);

  if (selected_button < 5)
    draw_trans_sprite (buffer, menuselect, 50, selector_y);

  // Buttons
  for (int i = 0; i < NUM_BUTTONS; i++)
    buttons[i].Draw(buffer);

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
