#include "TileMap.h"

#include <sstream>
#include <fstream>

#include "globals.h"
#include "utility/tools.h"


// Constructor
TileMap::TileMap (const std::string &file) {
  width = 0;
  height = 0;
  lighting = false;
  frame_timer.Start();

  if (file != "")
    load (file);
}

// Destructor
TileMap::~TileMap() { }

// Get width
int TileMap::getWidth() const {
  return width * 64;
}

// Get height
int TileMap::getHeight() const {
  return height * 64;
}

// Get frame
int TileMap::getFrame() {
  return int (frame_timer.GetElapsedTime<milliseconds>() / 100) % 8;
}

// Has lighting enabled
bool TileMap::hasLighting() const {
  return lighting;
}

// Toggle lighting effects
void TileMap::toggleLights() {
  lighting = !lighting;
}

void TileMap::create (int width, int height) {
  mapTiles.clear();
  mapTilesBack.clear();
  this -> width = width;
  this -> height = height;
  this -> lighting = false;

  for (int t = 0; t < height; t++) {
    for (int i = 0; i < width; i++) {
      mapTiles.push_back (Tile (0, i * 64, t * 64));
      mapTilesBack.push_back (Tile (0, i * 64, t * 64));
    }
  }
}

void TileMap::load_layer (std::ifstream &file, std::vector<Tile> &t_map) {
  // Unompress similar tiles
  unsigned char type_count = 0;
  unsigned short type = 0;
  int position = 0;

  while (position < width * height) {
    file.read ((char *) (&type_count), sizeof (type_count));
    file.read ((char *) (&type), sizeof (type));

    for (int i = 0; i < type_count; i++) {
      t_map.push_back (Tile (type, (position % width) * 64, (position / width) * 64));
      position ++;
    }
  }
}

bool TileMap::load (const std::string &file) {
  //Change size
  std::ifstream rf ((file + ".level").c_str(), std::ios::in | std::ios::binary);

  if (rf.fail()) {
    rf.close();
    return false;
  }

  //Setup Map
  mapTiles.clear();
  mapTilesBack.clear();
  width = 0;
  height = 0;
  lighting = false;

  // Dimensions
  rf.seekg (0);
  rf.read ((char *) (&width), sizeof (width));
  rf.seekg (4);
  rf.read ((char *) (&height), sizeof (height));
  rf.seekg (8);
  rf.read ((char *) (&lighting), sizeof (lighting));

  rf.seekg (32);

  load_layer (rf, mapTiles);
  load_layer (rf, mapTilesBack);

  rf.close();

  return true;
}

void TileMap::save_layer (std::ofstream &file, std::vector<Tile> &t_map) {
  unsigned char type_count = 0;
  unsigned short type = 0;

  // Compress similar tiles
  for (auto &t : t_map) {
    if ((t.getType() != type || type_count == 255) && type_count != 0) {
      file.write ((char *) (&type_count), sizeof (type_count));
      file.write ((char *) (&type), sizeof (type));
      type_count = 0;
    }

    type = t.getType();
    type_count ++;
  }

  file.write ((char *) (&type_count), sizeof (type_count));
  file.write ((char *) (&type), sizeof (type));
}

// Save file
void TileMap::save (const std::string &file) {
  std::ofstream of ((file + ".level").c_str(), std::ios::out | std::ios::binary);

  if (of.fail()) {
    of.close();
    return;
  }

  // Dimensions
  of.seekp (0);
  of.write ((char *)&width, sizeof (width));
  of.seekp (4);
  of.write ((char *)&height, sizeof (height));
  of.seekp (8);
  of.write ((char *)&lighting, sizeof (int));
  of.seekp (32);

  // Layers
  save_layer (of, mapTiles);
  save_layer (of, mapTilesBack);

  of.close();
}

// Get tile at
Tile *TileMap::get_tile_at (int s_x, int s_y, int layer) {
  std::vector<Tile> *ttm = (layer == 1) ? &mapTiles : &mapTilesBack;

  for (auto &t : *ttm) {
    if (collisionAny (s_x, s_x, t.getX(), t.getX() + 64,
                      s_y, s_y, t.getY(), t.getY() + 64)) {
      return &t;
    }
  }

  return nullptr;
}

// Find tile type
Tile *TileMap::find_tile_type (int type, int layer) {
  std::vector<Tile> *ttm = (layer == 1) ? &mapTiles : &mapTilesBack;

  for (auto &t : *ttm) {
    if (t.getType() == type) {
      return &t;
    }
  }

  return nullptr;
}

// Get tile at
std::vector<Tile *> TileMap::get_tiles_in_range (int x_1, int x_2, int y_1, int y_2) {
  std::vector<Tile *> ranged_map;

  for (auto &t : mapTiles) {
    if (t.getType() != 0 &&
        collisionAny (x_1, x_2, t.getX(), t.getX() + t.getWidth(),
                      y_1, y_2, t.getY(), t.getY() + t.getHeight())) {
      ranged_map.push_back (&t);
    }
  }

  return ranged_map;
}

// Draw a layer
void TileMap::draw_layer (BITMAP *buffer, std::vector<Tile> &t_map, int x, int y) {
  int frame = getFrame();

  for (auto &t : t_map) {
    if ((t.getX() + t.getWidth() >= x) && (t.getX() < x + NATIVE_SCREEN_W) &&
        (t.getY() + t.getHeight() >= y) && (t.getY() < y + NATIVE_SCREEN_H)) {
      t.draw (buffer, x, y, frame);
    }
  }
}

// Draw at position
void TileMap::draw (BITMAP *buffer, int x, int y, int layer) {
  if (layer == 0 || layer == 1) {
    draw_layer (buffer, mapTilesBack, x, y);
  }

  if (layer == 0 || layer == 2) {
    draw_layer (buffer, mapTiles, x, y);
  }
}
