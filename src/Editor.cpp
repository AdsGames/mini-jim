#include "Editor.h"

#include <string>

#include "globals.h"
#include "utility/tools.h"
#include "utility/KeyListener.h"

Editor::Editor() {
  // Other Sprites
  buffer = create_bitmap (SCREEN_W, SCREEN_H);

  // Create map
  tile_map = new TileMap ("data/templates/blank64x48");

  // Create example tile
  pallette_tile = new tile (0, 0, 0);

  font = load_font_ex ("fonts/arial_black.pcx");

  ib_save = InputBox(400, 408, 500, 50, ".txt");
  ib_open = InputBox(400, 408, 500, 50, ".txt");

  btn_save = Button(540, 500);
  btn_open = Button(540, 500);
  btn_save.SetImages("images/gui/button_save.png", "images/gui/button_save_hover.png");
  btn_open.SetImages("images/gui/button_load.png", "images/gui/button_load_hover.png");

  set_alpha_blender();

  layer = 1;
  opening = false;
  saving = false;
}

Editor::~Editor() {
  destroy_bitmap (buffer);

  delete tile_map;
  delete pallette_tile;
}

void Editor::save() {
  ib_save.Focus();
  ib_save.Update();

  // Save
  if (mouse_b & 1 && btn_save.Hover()) {
    tile_map -> save ("data/" + ib_save.GetValue());
    saving = false;
  }
}

void Editor::open() {
  ib_open.Focus();
  ib_open.Update();

  // Load file
  if (mouse_b & 1 && btn_open.Hover()) {
    tile_map -> load ("data/" + ib_open.GetValue().substr (0, ib_open.GetValue().size() - 4));
    opening = false;
  }
}

void Editor::edit() {
  // Scroll Map
  if (mouse_y < 10 && tile_map -> y > 0)
    tile_map -> y -= 16;
  else if (mouse_y > (SCREEN_H - 10) && tile_map -> y < tile_map -> height * 64 -  SCREEN_H)
    tile_map -> y += 16;
  else if (mouse_x  < 10 && tile_map -> x > 0)
    tile_map -> x -= 8;
  else if (mouse_x   > (SCREEN_W - 10) && tile_map -> x < tile_map -> width * 64 - SCREEN_W)
    tile_map -> x += 8;

  // Change selected
  if (KeyListener::keyPressed[KEY_UP]) {
    pallette_tile -> setType (pallette_tile -> getType() + 1);
    while (pallette_tile -> getImage() == nullptr && pallette_tile -> getType() < 400) {
      pallette_tile -> setType (pallette_tile -> getType() + 1);
    }
  }

  if (KeyListener::keyPressed[KEY_DOWN]) {
    pallette_tile -> setType (pallette_tile -> getType() - 1);
    while (pallette_tile -> getImage() == nullptr && pallette_tile -> getType() > 1) {
      pallette_tile -> setType (pallette_tile -> getType() - 1);
    }
  }

  // Change Layer
  if (KeyListener::keyPressed[KEY_TAB])
    layer = !layer;

  // Operations
  tile *temp_tile = tile_map -> get_tile_at(mouse_x, mouse_y, layer);
  if (temp_tile) {
    // Place tile
    if (mouse_b & 1)
      temp_tile -> setType (pallette_tile -> getType());

    // Erase tile
    if (mouse_b & 2)
      temp_tile -> setType (0);

    // Get tile type tile
    if (KeyListener::keyPressed[KEY_K])
      pallette_tile -> setType(temp_tile -> getType());
  }

  // Save map
  if (KeyListener::keyPressed[KEY_S]) {
    clear_keybuf();
    saving = true;
  }

  // Open map
  if (KeyListener::keyPressed[KEY_O]) {
    clear_keybuf();
    opening = true;
  }

  //Fill map
  if (KeyListener::keyPressed[KEY_F]) {
    for (auto &t: tile_map -> mapTilesBack) {
      t.setType (pallette_tile -> getType());
    }
  }
}

void Editor::update() {
  // Back to menu
  if (KeyListener::keyPressed[KEY_M] && !saving && !opening) {
    set_next_state (STATE_MENU);
  }

  // Close menu
  if (KeyListener::keyPressed[KEY_TILDE]) {
    opening = false;
    saving = false;
  }

  // Run states
  if (saving)
    save();
  else if (opening)
    open();
  else
    edit();
}

void Editor::draw() {
  // Background
  rectfill (buffer, 0, 0, SCREEN_W, SCREEN_H, makecol (255, 255, 255));

  // Draw tiles
  tile_map -> draw (buffer);
  pallette_tile -> draw_tile (buffer, 0, 0, 0);

  // Map info
  textprintf_ex (buffer, font, 0, 80, makecol (255, 255, 255), makecol (0, 0, 0), "height-%i width-%i", tile_map -> height, tile_map -> width);
  if (layer == 1)
    textprintf_ex (buffer, font, 0, 130, makecol (255, 255, 255), makecol (0, 0, 0), "Editing Mode: Foreground");
  else
    textprintf_ex (buffer, font, 0, 130, makecol (255, 255, 255), makecol (0, 0, 0), "Editing Mode: Background");

  if (saving) {
    rectfill (buffer, 330, 300, SCREEN_W - 330, SCREEN_H - 400, makecol (255, 255, 255));
    rect (buffer, 330, 300, SCREEN_W - 330, SCREEN_H - 400, makecol (0, 0, 0));
    textprintf_centre_ex (buffer, font, 640, 310, makecol (0, 0, 0), -1, "Save Map Name");
    ib_save.Draw(buffer);
    btn_save.Draw(buffer);
  }
  else if (opening) {
    rectfill (buffer, 330, 300, SCREEN_W - 330, SCREEN_H - 400, makecol (255, 255, 255));
    rect (buffer, 330, 300, SCREEN_W - 330, SCREEN_H - 400, makecol (0, 0, 0));
    textprintf_centre_ex (buffer, font, 640, 310, makecol (0, 0, 0), -1, "Open Map Name");
    ib_open.Draw(buffer);
    btn_open.Draw(buffer);
  }

  // Cursor
  circlefill (buffer, mouse_x, mouse_y, 10, makecol (0, 0, 0));
  circlefill (buffer, mouse_x, mouse_y, 8, makecol (255, 255, 255));

  // Draw buffer
  stretch_sprite (screen, buffer, 0, 0, SCREEN_W, SCREEN_H);
}
