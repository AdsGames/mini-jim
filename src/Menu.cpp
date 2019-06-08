#include "Menu.h"

#include "utility/KeyListener.h"
#include "utility/MouseListener.h"

// Create menu
Menu::Menu() {
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
  next_state = -1;

  // Buttons
  buttons[BUTTON_START] = Button(60, 630);
  buttons[BUTTON_START_MP] = Button(60, 690);
  buttons[BUTTON_EDIT] = Button(60, 750);
  buttons[BUTTON_HELP] = Button(60, 810);
  buttons[BUTTON_EXIT] = Button(60, 870);
  buttons[BUTTON_LEFT] = Button(NATIVE_SCREEN_W - 180, 80);
  buttons[BUTTON_RIGHT] = Button(NATIVE_SCREEN_W - 80, 80);

  buttons[BUTTON_START].SetImages("images/gui/button_start.png", "images/gui/button_start_hover.png");
  buttons[BUTTON_START_MP].SetImages("images/gui/button_start_mp.png", "images/gui/button_start_mp_hover.png");
  buttons[BUTTON_EDIT].SetImages("images/gui/button_edit.png", "images/gui/button_edit_hover.png");
  buttons[BUTTON_HELP].SetImages("images/gui/button_help.png", "images/gui/button_help_hover.png");
  buttons[BUTTON_EXIT].SetImages("images/gui/button_quit.png", "images/gui/button_quit_hover.png");
  buttons[BUTTON_LEFT].SetImages("images/gui/button_left.png", "images/gui/button_left_hover.png");
  buttons[BUTTON_RIGHT].SetImages("images/gui/button_right.png", "images/gui/button_right_hover.png");

  buttons[BUTTON_START].SetOnClick([this]() {
    single_player = true;
    next_state = StateEngine::STATE_GAME;
  });

  buttons[BUTTON_START_MP].SetOnClick([this]() {
    single_player = false;
    next_state = StateEngine::STATE_GAME;
  });

  buttons[BUTTON_EDIT].SetOnClick([this]() {
    next_state = StateEngine::STATE_EDIT;
  });

  buttons[BUTTON_EXIT].SetOnClick([this]() {
    next_state = StateEngine::STATE_EXIT;
  });

  buttons[BUTTON_LEFT].SetOnClick([this]() {
    change_level(-1);
  });

  buttons[BUTTON_RIGHT].SetOnClick([this]() {
    change_level(1);
  });

  //Variables
  play_sample (music, 255, 125, 1000, 1);
  play_sample (intro, 255, 128, 1000, 0);
}

Menu::~Menu() {
  // Destory Bitmaps
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

bool Menu::button_hover() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    if (buttons[i].Hover()) {
      return true;
    }
  }
  return false;
}

void Menu::change_level(int level) {
  levelOn = (levelOn + level) < 0 ? 4 : (levelOn + level) % 5;

  tile_map -> load ("data/level_" + std::to_string(levelOn + 1));

  scroll_x = random(NATIVE_SCREEN_W, tile_map -> getWidth() - NATIVE_SCREEN_W);
  scroll_dir_x = random (0, 1) ? -3 : 3;
  scroll_y = random(NATIVE_SCREEN_H, tile_map -> getHeight() - NATIVE_SCREEN_H);
  scroll_dir_y = random (0, 1) ? -3 : 3;

  play_sample (click, 255, 125, 1000, 0);

  cam = Camera(NATIVE_SCREEN_W, NATIVE_SCREEN_H, tile_map -> getWidth(), tile_map -> getHeight());
  cam.SetSpeed(5);
}

void Menu::update(StateEngine *engine) {
  poll_joystick();

  // Move around live background
  if (scroll_x + NATIVE_SCREEN_W / 2 >= tile_map -> getWidth() || scroll_x <= NATIVE_SCREEN_W / 2)
    scroll_dir_x *= -1;
  if (scroll_y + NATIVE_SCREEN_H / 2 >= tile_map -> getHeight() || scroll_y <= NATIVE_SCREEN_H / 2)
    scroll_dir_y *= -1;
  scroll_x += scroll_dir_x;
  scroll_y += scroll_dir_y;

  cam.Follow(scroll_x, scroll_y);

  // State change
  if (next_state != -1)
    setNextState (engine, next_state);

  // Buttons
  for (int i = 0; i < NUM_BUTTONS; i++)
    buttons[i].Update();
}

void Menu::draw(BITMAP *buffer) {
  // Draw background to screen
  rectfill (buffer, 0, 0, NATIVE_SCREEN_W, NATIVE_SCREEN_H, makecol (255, 255, 255));

  // Draw live background
  tile_map -> draw (buffer, cam.GetX(), cam.GetY());

  // Overlay
  draw_trans_sprite (buffer, credits, 0, 0);
  draw_trans_sprite (buffer, menu, 40, 480);

  // Buttons
  for (int i = 0; i < NUM_BUTTONS; i++)
    buttons[i].Draw(buffer);

  // Level selection
  draw_trans_sprite (buffer, levelSelectNumber, NATIVE_SCREEN_W - 160, 80);
  textprintf_centre_ex (buffer, font, NATIVE_SCREEN_W - 113, 76, 0x000000, -1, "%i", levelOn + 1);

  // Cursor
  draw_sprite (buffer, cursor, MouseListener::x, MouseListener::y);

  // Help menu
  if (buttons[BUTTON_HELP].Hover())
    draw_trans_sprite (buffer, help, 0, 0);

  draw_trans_sprite (buffer, copyright, NATIVE_SCREEN_W - 350, NATIVE_SCREEN_H - 40);
  draw_sprite (buffer, cursor, MouseListener::x, MouseListener::y);
}
