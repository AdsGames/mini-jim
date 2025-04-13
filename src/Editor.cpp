#include "Editor.h"

#include <string>

#include "TileTypeLoader.h"
#include "globals.h"
#include "utility/tools.h"

void Editor::init() {
  tile_map = std::make_unique<TileMap>();
  pallette_tile = std::make_unique<Tile>(0, 0, 0);

  font_editor = asw::assets::loadFont("assets/fonts/ariblk.ttf", 24);

  ib_save = InputBox(400, 408, 480, 50, font_editor, "untitled");
  ib_open = InputBox(400, 408, 480, 50, font_editor, "level_1");
  ib_width = InputBox(400, 408, 220, 50, font_editor, "64", "number");
  ib_height = InputBox(660, 408, 220, 50, font_editor, "64", "number");

  btn_save = Button({400, 500});
  btn_open = Button({400, 500});
  btn_new = Button({400, 500});
  btn_close = Button({663, 500});

  btn_save.SetOnClick(std::bind(&Editor::save_map, this));
  btn_open.SetOnClick(std::bind(&Editor::open_map, this));
  btn_new.SetOnClick(std::bind(&Editor::new_map, this));
  btn_close.SetOnClick(std::bind(&Editor::close_map, this));

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
  editor_state = EditorState::Open;
}

void Editor::close_map() {
  editor_state = EditorState::Edit;
}

void Editor::save_map() {
  tile_map->save("assets/data/" + ib_save.GetValue());
  editor_state = EditorState::Edit;
}

void Editor::open_map() {
  tile_map->load("assets/data/" + ib_open.GetValue());
  editor_state = EditorState::Edit;
  auto size = asw::display::getLogicalSize();
  cam = Camera(size.x, size.y, tile_map->getWidth(), tile_map->getHeight());
  cam.SetSpeed(1);
  cam.SetBounds(20, 20);
}

void Editor::new_map() {
  if (ib_width.GetValue().length() != 0 && ib_width.GetValue().length() != 0) {
    editor_state = EditorState::Edit;
    tile_map->create(stoi(ib_width.GetValue()), stoi(ib_height.GetValue()));
    auto size = asw::display::getLogicalSize();

    cam = Camera(size.x, size.y, tile_map->getWidth(), tile_map->getHeight());
    cam.SetSpeed(1);
    cam.SetBounds(20, 20);
  }
}

void Editor::edit_map() {
  cam.Follow(static_cast<float>(asw::input::mouse.x) + cam.GetX(),
             static_cast<float>(asw::input::mouse.y) + cam.GetY());

  // Change selected
  if (asw::input::keyboard.pressed[SDL_SCANCODE_UP]) {
    int i = pallette_tile->getType() + 1;

    while (TileTypeLoader::getTile(i) == nullptr) {
      i = (i + 1) > 400 ? 0 : i + 1;
    }

    pallette_tile->setType(i);
  }

  if (asw::input::keyboard.pressed[SDL_SCANCODE_DOWN]) {
    int i = pallette_tile->getType() - 1;

    while (TileTypeLoader::getTile(i) == nullptr) {
      i = (i - 1) < 0 ? 400 : i - 1;
    }

    pallette_tile->setType(i);
  }

  // Change Layer
  if (asw::input::keyboard.pressed[SDL_SCANCODE_TAB]) {
    layer = static_cast<int>(!static_cast<bool>(layer));
    draw_layer = layer + 1;
  }

  // Toggle lights
  if (asw::input::keyboard.pressed[SDL_SCANCODE_L]) {
    tile_map->toggleLights();
  }

  // Operations
  Tile* temp_tile =
      tile_map->get_tile_at(asw::input::mouse.x + cam.GetX(),
                            asw::input::mouse.y + cam.GetY(), layer);

  if (temp_tile != nullptr) {
    // Place tile
    if (asw::input::mouse.down[1]) {
      temp_tile->setType(pallette_tile->getType());
    }

    // Erase tile
    if (asw::input::mouse.down[4]) {
      temp_tile->setType(0);
    }

    // Get tile type tile
    if (asw::input::keyboard.pressed[SDL_SCANCODE_K])
      pallette_tile->setType(temp_tile->getType());
  }

  // Save map
  if (asw::input::keyboard.pressed[SDL_SCANCODE_S]) {
    ib_save.Focus();
    editor_state = EditorState::Save;
  }

  // Open map
  if (asw::input::keyboard.pressed[SDL_SCANCODE_O]) {
    ib_open.Focus();
    editor_state = EditorState::Open;
  }

  // New map
  if (asw::input::keyboard.pressed[SDL_SCANCODE_N]) {
    ib_width.Update();
    editor_state = EditorState::Create;
  }

  // Fill map
  if (asw::input::keyboard.pressed[SDL_SCANCODE_F]) {
    for (auto& t : tile_map->mapTilesBack) {
      t.setType(pallette_tile->getType());
    }
  }

  // Draw specific layers
  if (asw::input::keyboard.down[SDL_SCANCODE_0]) {
    draw_layer = 0;
  }
}

void Editor::update() {
  // Back to menu
  if (asw::input::keyboard.pressed[SDL_SCANCODE_M] &&
      editor_state == EditorState::Edit) {
    setNextState(ProgramState::Menu);
  }

  // Run states
  if (editor_state == EditorState::Save) {
    ib_save.Update();
    btn_save.Update();
    btn_close.Update();
  } else if (editor_state == EditorState::Open) {
    ib_open.Update();
    btn_open.Update();
    btn_close.Update();
  } else if (editor_state == EditorState::Create) {
    ib_width.Update();
    ib_height.Update();
    btn_new.Update();
    btn_close.Update();
  } else {
    edit_map();
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
  asw::draw::text(font_editor, pallette_tile->getName(),
                  asw::Vec2<float>(70, 20),
                  asw::util::makeColor(255, 255, 255));

  // Map info
  asw::draw::text(font_editor,
                  "height-" + std::to_string(tile_map->getHeight()) +
                      " width-" + std::to_string(tile_map->getWidth()) +
                      " lighting-" + std::to_string(tile_map->hasLighting()),
                  asw::Vec2<float>(0, 80), asw::util::makeColor(255, 255, 255));

  if (layer == 1)
    asw::draw::text(font_editor, "Editing Mode: Foreground ",
                    asw::Vec2<float>(0, 130),
                    asw::util::makeColor(255, 255, 255));
  else
    asw::draw::text(font_editor, "Editing Mode: Background ",
                    asw::Vec2<float>(0, 130),
                    asw::util::makeColor(255, 255, 255));

  if (editor_state == EditorState::Save) {
    asw::draw::rectFill(
        asw::Quad<float>(330, 300, screenSize.x - 660, screenSize.y - 600),
        asw::util::makeColor(255, 255, 255, 255));
    asw::draw::rect(
        asw::Quad<float>(330, 300, screenSize.x - 330, screenSize.y - 400),
        asw::util::makeColor(0, 0, 0));
    asw::draw::textCenter(font_editor, "Save Map Name",
                          asw::Vec2<float>(640, 310),
                          asw::util::makeColor(0, 0, 0));
    ib_save.Draw();
    btn_save.Draw();
    btn_close.Draw();
  } else if (editor_state == EditorState::Open) {
    asw::draw::rectFill(
        asw::Quad<float>(330, 300, screenSize.x - 660, screenSize.y - 600),
        asw::util::makeColor(255, 255, 255, 255));
    asw::draw::rect(
        asw::Quad<float>(330, 300, screenSize.x - 330, screenSize.y - 400),
        asw::util::makeColor(0, 0, 0));
    asw::draw::textCenter(font_editor, "Open Map Name",
                          asw::Vec2<float>(640, 310),
                          asw::util::makeColor(0, 0, 0));
    ib_open.Draw();
    btn_open.Draw();
    btn_close.Draw();
  } else if (editor_state == EditorState::Create) {
    asw::draw::rectFill(
        asw::Quad<float>(330, 300, screenSize.x - 660, screenSize.y - 600),
        asw::util::makeColor(255, 255, 255, 255));
    asw::draw::rect(
        asw::Quad<float>(330, 300, screenSize.x - 330, screenSize.y - 400),
        asw::util::makeColor(0, 0, 0));
    asw::draw::textCenter(font_editor, "New Map", asw::Vec2<float>(640, 310),
                          asw::util::makeColor(0, 0, 0));
    asw::draw::textCenter(font_editor, "Width", asw::Vec2<float>(500, 360),
                          asw::util::makeColor(0, 0, 0));
    asw::draw::textCenter(font_editor, "Height", asw::Vec2<float>(800, 360),
                          asw::util::makeColor(0, 0, 0));
    ib_width.Draw();
    ib_height.Draw();
    btn_new.Draw();
    btn_close.Draw();
  }
}
