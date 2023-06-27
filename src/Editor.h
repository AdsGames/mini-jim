#pragma once

#include "State.h"

#include <asw/asw.h>
#include <memory>

#include "Camera.h"
#include "Tile.h"
#include "TileMap.h"
#include "ui/Button.h"
#include "ui/InputBox.h"

enum class EditorState { Save, Open, Create, Edit };

class Editor : public State {
 public:
  explicit Editor(StateEngine& engine) : State(engine) {}

  virtual void init() override;
  virtual void update() override;
  virtual void draw() override;
  virtual void cleanup() override{};

 private:
  int layer;
  int draw_layer;

  EditorState editor_state;

  std::unique_ptr<TileMap> tile_map;
  std::unique_ptr<Tile> pallette_tile;

  asw::Font font_editor;

  // Text input
  Button btn_save;
  Button btn_open;
  Button btn_new;
  Button btn_close;

  InputBox ib_save;
  InputBox ib_open;
  InputBox ib_width;
  InputBox ib_height;

  void close_map();
  void save_map();
  void open_map();
  void new_map();
  void edit_map();

  Camera cam;
};
