#include "Editor.h"

#include <string>
#include "TileTypeLoader.h"

#include "globals.h"
#include "utility/tools.h"
#include "utility/MouseListener.h"
#include "utility/KeyListener.h"

Editor::Editor() {
  // Create map
  tile_map = new TileMap();

  // Create example tile
  pallette_tile = new Tile (0, 0, 0);

  font = load_font_ex ("fonts/arial_black.pcx");
  cursor = load_png_ex ("images/gui/cursor1.png");

  ib_save = InputBox (400, 408, 480, 50, "untitled");
  ib_open = InputBox (400, 408, 480, 50, "level_1");
  ib_width = InputBox (400, 408, 220, 50, "64", "number");
  ib_height = InputBox (660, 408, 220, 50, "64", "number");

  btn_save = Button (400, 500);
  btn_open = Button (400, 500);
  btn_new = Button (400, 500);
  btn_close = Button (663, 500);

  btn_save.SetOnClick (std::bind (&Editor::Save, this));
  btn_open.SetOnClick (std::bind (&Editor::Open, this));
  btn_new.SetOnClick (std::bind (&Editor::New, this));
  btn_close.SetOnClick (std::bind (&Editor::Close, this));

  btn_save.SetImages ("images/gui/button_save.png", "images/gui/button_save_hover.png");
  btn_open.SetImages ("images/gui/button_load.png", "images/gui/button_load_hover.png");
  btn_new.SetImages ("images/gui/button_create.png", "images/gui/button_create_hover.png");
  btn_close.SetImages ("images/gui/button_close.png", "images/gui/button_close_hover.png");

  set_alpha_blender();

  layer = 1;
  draw_layer = 2;
  editor_state = OPEN;
}

Editor::~Editor() {
  destroy_bitmap (cursor);

  delete tile_map;
  delete pallette_tile;
}

void Editor::Close() {
  editor_state = EDIT;
}

void Editor::Save() {
  tile_map -> save ("data/" + ib_save.GetValue());
  editor_state = EDIT;
}

void Editor::Open() {
  tile_map -> load ("data/" + ib_open.GetValue());
  editor_state = EDIT;
  cam = Camera (NATIVE_SCREEN_W, NATIVE_SCREEN_H, tile_map -> getWidth(), tile_map -> getHeight());
  cam.SetSpeed (1);
  cam.SetBounds (20, 20);
}

void Editor::New() {
  if (ib_width.GetValue().length() != 0 && ib_width.GetValue().length() != 0) {
    editor_state = EDIT;
    tile_map -> create (stoi (ib_width.GetValue()), stoi (ib_height.GetValue()));
    cam = Camera (NATIVE_SCREEN_W, NATIVE_SCREEN_H, tile_map -> getWidth(), tile_map -> getHeight());
    cam.SetSpeed (1);
    cam.SetBounds (20, 20);
  }
}

void Editor::Edit() {
  cam.Follow (MouseListener::x + cam.GetX(), MouseListener::y + cam.GetY());

  // Change selected
  if (KeyListener::keyPressed[KEY_UP]) {
    int i = pallette_tile -> getType() + 1;

    while (!TileTypeLoader::GetTile (i))
      i = (i + 1) > 400 ? 0 : i + 1;

    pallette_tile -> setType (i);
  }

  if (KeyListener::keyPressed[KEY_DOWN]) {
    int i = pallette_tile -> getType() - 1;

    while (!TileTypeLoader::GetTile (i))
      i = (i - 1) < 0 ? 400 : i - 1;

    pallette_tile -> setType (i);
  }

  // Change Layer
  if (KeyListener::keyPressed[KEY_TAB]) {
    layer = !layer;
    draw_layer = layer + 1;
  }

  // Toggle lights
  if (KeyListener::keyPressed[KEY_L]) {
    tile_map -> toggleLights();
  }

  // Operations
  Tile *temp_tile = tile_map -> get_tile_at (MouseListener::x + cam.GetX(), MouseListener::y + cam.GetY(), layer);

  if (temp_tile) {
    // Place tile
    if (mouse_b & 1)
      temp_tile -> setType (pallette_tile -> getType());

    // Erase tile
    if (mouse_b & 2)
      temp_tile -> setType (0);

    // Get tile type tile
    if (KeyListener::keyPressed[KEY_K])
      pallette_tile -> setType (temp_tile -> getType());
  }

  // Save map
  if (KeyListener::keyPressed[KEY_S]) {
    clear_keybuf();
    ib_save.Focus();
    editor_state = SAVE;
  }

  // Open map
  if (KeyListener::keyPressed[KEY_O]) {
    clear_keybuf();
    ib_open.Focus();
    editor_state = OPEN;
  }

  // New map
  if (KeyListener::keyPressed[KEY_N]) {
    clear_keybuf();
    ib_width.Update();
    editor_state = CREATE;
  }

  //Fill map
  if (KeyListener::keyPressed[KEY_F]) {
    for (auto &t : tile_map -> mapTilesBack) {
      t.setType (pallette_tile -> getType());
    }
  }

  // Draw specific layers
  if (key[KEY_0])
    draw_layer = 0;
}

void Editor::update (StateEngine &engine) {
  // Back to menu
  if (KeyListener::keyPressed[KEY_M] && editor_state == EDIT) {
    setNextState (engine, StateEngine::STATE_MENU);
  }

  // Run states
  if (editor_state == SAVE) {
    ib_save.Update();
    btn_save.Update();
    btn_close.Update();
  }
  else if (editor_state == OPEN) {
    ib_open.Update();
    btn_open.Update();
    btn_close.Update();
  }
  else if (editor_state == CREATE) {
    ib_width.Update();
    ib_height.Update();
    btn_new.Update();
    btn_close.Update();
  }
  else {
    Edit();
  }
}

void Editor::draw (BITMAP *buffer) {
  // Background
  clear_to_color (buffer, 0x000000);

  // Draw tiles
  tile_map -> draw (buffer, cam.GetX(), cam.GetY(), draw_layer);

  pallette_tile -> draw (buffer, 0, 0, 0);
  textprintf_ex (buffer, font, 70, 20, makecol (255, 255, 255), makecol (0, 0, 0), "%s", pallette_tile -> getName().c_str());

  // Map info
  textprintf_ex (buffer, font, 0, 80, makecol (255, 255, 255), makecol (0, 0, 0), "height-%i width-%i lighting-%i", tile_map -> getHeight(), tile_map -> getWidth(), tile_map -> hasLighting());

  if (layer == 1)
    textprintf_ex (buffer, font, 0, 130, makecol (255, 255, 255), makecol (0, 0, 0), "Editing Mode: Foreground");
  else
    textprintf_ex (buffer, font, 0, 130, makecol (255, 255, 255), makecol (0, 0, 0), "Editing Mode: Background");

  if (editor_state == SAVE) {
    rectfill (buffer, 330, 300, NATIVE_SCREEN_W - 330, NATIVE_SCREEN_H - 400, makecol (255, 255, 255));
    rect (buffer, 330, 300, NATIVE_SCREEN_W - 330, NATIVE_SCREEN_H - 400, makecol (0, 0, 0));
    textprintf_centre_ex (buffer, font, 640, 310, makecol (0, 0, 0), -1, "Save Map Name");
    ib_save.Draw (buffer);
    btn_save.Draw (buffer);
    btn_close.Draw (buffer);
  }
  else if (editor_state == OPEN) {
    rectfill (buffer, 330, 300, NATIVE_SCREEN_W - 330, NATIVE_SCREEN_H - 400, makecol (255, 255, 255));
    rect (buffer, 330, 300, NATIVE_SCREEN_W - 330, NATIVE_SCREEN_H - 400, makecol (0, 0, 0));
    textprintf_centre_ex (buffer, font, 640, 310, makecol (0, 0, 0), -1, "Open Map Name");
    ib_open.Draw (buffer);
    btn_open.Draw (buffer);
    btn_close.Draw (buffer);
  }
  else if (editor_state == CREATE) {
    rectfill (buffer, 330, 300, NATIVE_SCREEN_W - 330, NATIVE_SCREEN_H - 400, makecol (255, 255, 255));
    rect (buffer, 330, 300, NATIVE_SCREEN_W - 330, NATIVE_SCREEN_H - 400, makecol (0, 0, 0));
    textprintf_centre_ex (buffer, font, 640, 310, makecol (0, 0, 0), -1, "New Map");
    textprintf_centre_ex (buffer, font, 500, 360, makecol (0, 0, 0), -1, "Width");
    textprintf_centre_ex (buffer, font, 800, 360, makecol (0, 0, 0), -1, "Height");
    ib_width.Draw (buffer);
    ib_height.Draw (buffer);
    btn_new.Draw (buffer);
    btn_close.Draw (buffer);
  }

  // Cursor
  draw_sprite (buffer, cursor, MouseListener::x, MouseListener::y);
}
