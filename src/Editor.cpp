#include "Editor.h"

#include <asw/util/KeyListener.h>
#include <asw/util/MouseListener.h>
#include <string>

#include "TileTypeLoader.h"
#include "globals.h"
#include "utility/tools.h"

void Editor::init() {
  // Create map
  tile_map = new TileMap();

  // Create example tile
  pallette_tile = new Tile(0, 0, 0);

  editorFont = asw::load::font("assets/fonts/ariblk.ttf", 24);

  ib_save = InputBox(400, 408, 480, 50, editorFont, "untitled");
  ib_open = InputBox(400, 408, 480, 50, editorFont, "level_1");
  ib_width = InputBox(400, 408, 220, 50, editorFont, "64", "number");
  ib_height = InputBox(660, 408, 220, 50, editorFont, "64", "number");

  btn_save = Button(400, 500);
  btn_open = Button(400, 500);
  btn_new = Button(400, 500);
  btn_close = Button(663, 500);

  btn_save.SetOnClick(std::bind(&Editor::Save, this));
  btn_open.SetOnClick(std::bind(&Editor::Open, this));
  btn_new.SetOnClick(std::bind(&Editor::New, this));
  btn_close.SetOnClick(std::bind(&Editor::Close, this));

  btn_save.SetImages("assets/images/gui/button_save.png",
                     "assets/images/gui/button_save_hover.png");
  btn_open.SetImages("assets/images/gui/button_load.png",
                     "assets/images/gui/button_load_hover.png");
  btn_new.SetImages("assets/images/gui/button_create.png",
                    "assets/images/gui/button_create_hover.png");
  btn_close.SetImages("assets/images/gui/button_close.png",
                      "assets/images/gui/button_close_hover.png");

  // set_alpha_blender();

  layer = 1;
  draw_layer = 2;
  editor_state = OPEN;
}

void Editor::cleanup() {
  delete tile_map;
  delete pallette_tile;
}

void Editor::Close() {
  editor_state = EDIT;
}

void Editor::Save() {
  tile_map->save("assets/data/" + ib_save.GetValue());
  editor_state = EDIT;
}

void Editor::Open() {
  tile_map->load("assets/data/" + ib_open.GetValue());
  editor_state = EDIT;
  auto size = asw::display::getLogicalSize();
  cam = Camera(size.x, size.y, tile_map->getWidth(), tile_map->getHeight());
  cam.SetSpeed(1);
  cam.SetBounds(20, 20);
}

void Editor::New() {
  if (ib_width.GetValue().length() != 0 && ib_width.GetValue().length() != 0) {
    editor_state = EDIT;
    tile_map->create(stoi(ib_width.GetValue()), stoi(ib_height.GetValue()));
    auto size = asw::display::getLogicalSize();

    cam = Camera(size.x, size.y, tile_map->getWidth(), tile_map->getHeight());
    cam.SetSpeed(1);
    cam.SetBounds(20, 20);
  }
}

void Editor::Edit() {
  cam.Follow(MouseListener::x + cam.GetX(), MouseListener::y + cam.GetY());

  // Change selected
  if (KeyListener::keyPressed[SDL_SCANCODE_UP]) {
    int i = pallette_tile->getType() + 1;

    while (!TileTypeLoader::GetTile(i))
      i = (i + 1) > 400 ? 0 : i + 1;

    pallette_tile->setType(i);
  }

  if (KeyListener::keyPressed[SDL_SCANCODE_DOWN]) {
    int i = pallette_tile->getType() - 1;

    while (!TileTypeLoader::GetTile(i))
      i = (i - 1) < 0 ? 400 : i - 1;

    pallette_tile->setType(i);
  }

  // Change Layer
  if (KeyListener::keyPressed[SDL_SCANCODE_TAB]) {
    layer = !layer;
    draw_layer = layer + 1;
  }

  // Toggle lights
  if (KeyListener::keyPressed[SDL_SCANCODE_L]) {
    tile_map->toggleLights();
  }

  // Operations
  Tile* temp_tile = tile_map->get_tile_at(MouseListener::x + cam.GetX(),
                                          MouseListener::y + cam.GetY(), layer);

  if (temp_tile) {
    // Place tile
    if (MouseListener::mouse_button & 1)
      temp_tile->setType(pallette_tile->getType());

    // Erase tile
    if (MouseListener::mouse_button & 4)
      temp_tile->setType(0);

    // Get tile type tile
    if (KeyListener::keyPressed[SDL_SCANCODE_K])
      pallette_tile->setType(temp_tile->getType());
  }

  // Save map
  if (KeyListener::keyPressed[SDL_SCANCODE_S]) {
    ib_save.Focus();
    editor_state = SAVE;
  }

  // Open map
  if (KeyListener::keyPressed[SDL_SCANCODE_O]) {
    ib_open.Focus();
    editor_state = OPEN;
  }

  // New map
  if (KeyListener::keyPressed[SDL_SCANCODE_N]) {
    ib_width.Update();
    editor_state = CREATE;
  }

  // Fill map
  if (KeyListener::keyPressed[SDL_SCANCODE_F]) {
    for (auto& t : tile_map->mapTilesBack) {
      t.setType(pallette_tile->getType());
    }
  }

  // Draw specific layers
  if (KeyListener::keyDown[SDL_SCANCODE_0]) {
    draw_layer = 0;
  }
}

void Editor::update() {
  // Back to menu
  if (KeyListener::keyPressed[SDL_SCANCODE_M] && editor_state == EDIT) {
    setNextState(StateEngine::STATE_MENU);
  }

  // Run states
  if (editor_state == SAVE) {
    ib_save.Update();
    btn_save.Update();
    btn_close.Update();
  } else if (editor_state == OPEN) {
    ib_open.Update();
    btn_open.Update();
    btn_close.Update();
  } else if (editor_state == CREATE) {
    ib_width.Update();
    ib_height.Update();
    btn_new.Update();
    btn_close.Update();
  } else {
    Edit();
  }
}

void Editor::draw() {
  auto screenSize = asw::display::getLogicalSize();

  // Background
  asw::draw::clearColor(asw::util::makeColor(0, 0, 0));

  // Draw tiles
  tile_map->draw(cam.GetX(), cam.GetY(), screenSize.x, screenSize.y, 0, 0,
                 draw_layer);

  pallette_tile->draw(0, 0, 0);
  asw::draw::text(editorFont, pallette_tile->getName(), 70, 20,
                  asw::util::makeColor(255, 255, 255));

  // Map info
  asw::draw::text(editorFont,
                  "height-" + std::to_string(tile_map->getHeight()) +
                      " width-" + std::to_string(tile_map->getWidth()) +
                      " lighting-" + std::to_string(tile_map->hasLighting()),
                  0, 80, asw::util::makeColor(255, 255, 255));

  if (layer == 1)
    asw::draw::text(editorFont, "Editing Mode: Foreground ", 0, 130,
                    asw::util::makeColor(255, 255, 255));
  else
    asw::draw::text(editorFont, "Editing Mode: Background ", 0, 130,
                    asw::util::makeColor(255, 255, 255));

  if (editor_state == SAVE) {
    asw::draw::primRectFill(330, 300, screenSize.x - 330, screenSize.y - 400,
                            asw::util::makeColor(255, 255, 255, 255));
    asw::draw::primRect(330, 300, screenSize.x - 330, screenSize.y - 400,
                        asw::util::makeColor(0, 0, 0));
    asw::draw::textCenter(editorFont, "Save Map Name", 640, 310,
                          asw::util::makeColor(0, 0, 0));
    ib_save.Draw();
    btn_save.Draw();
    btn_close.Draw();
  } else if (editor_state == OPEN) {
    asw::draw::primRectFill(330, 300, screenSize.x - 330, screenSize.y - 400,
                            asw::util::makeColor(255, 255, 255, 255));
    asw::draw::primRect(330, 300, screenSize.x - 330, screenSize.y - 400,
                        asw::util::makeColor(0, 0, 0));
    asw::draw::textCenter(editorFont, "Open Map Name", 640, 310,
                          asw::util::makeColor(0, 0, 0));
    ib_open.Draw();
    btn_open.Draw();
    btn_close.Draw();
  } else if (editor_state == CREATE) {
    asw::draw::primRectFill(330, 300, screenSize.x - 330, screenSize.y - 400,
                            asw::util::makeColor(255, 255, 255, 255));
    asw::draw::primRect(330, 300, screenSize.x - 330, screenSize.y - 400,
                        asw::util::makeColor(0, 0, 0));
    asw::draw::textCenter(editorFont, "New Map", 640, 310,
                          asw::util::makeColor(0, 0, 0));
    asw::draw::textCenter(editorFont, "Width", 500, 360,
                          asw::util::makeColor(0, 0, 0));
    asw::draw::textCenter(editorFont, "Height", 800, 360,
                          asw::util::makeColor(0, 0, 0));
    ib_width.Draw();
    ib_height.Draw();
    btn_new.Draw();
    btn_close.Draw();
  }
}
