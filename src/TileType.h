#ifndef TILETYPE_H
#define TILETYPE_H

#include <allegro.h>
#include <string>
#include <vector>

class TileType {
 public:
  TileType(short id, const std::string& name, const std::string& id_str);
  virtual ~TileType();

  void Draw(BITMAP* buffer, int x, int y, int frame);

  short GetID() const;
  std::string GetName() const;
  std::string GetIDStr() const;

  int GetWidth() const;
  int GetHeight() const;

  int GetBBX() const;
  int GetBBY() const;

  bool HasAttribute(int attribute);

  void AddImage(BITMAP* image);
  void AddAttribute(int attribute);
  void SetDimensions(int x_1, int y_1, int x_2, int y_2);

 private:
  short id;
  std::string name;
  std::string id_str;
  int x_1, x_2, y_1, y_2;

  std::vector<int> attributes;
  std::vector<BITMAP*> images;
};

#endif  // TILETYPE_H
