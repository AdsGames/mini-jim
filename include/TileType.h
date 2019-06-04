#ifndef TILETYPE_H
#define TILETYPE_H

#include <allegro.h>
#include <vector>
#include <string>

class TileType {
  public:
    TileType(int id, std::string name, std::string id_str);
    virtual ~TileType();

    void Draw(BITMAP *buffer, int x, int y, int frame);

    int GetID();
    std::string GetName();
    std::string GetIDStr();

    int GetWidth();
    int GetHeight();

    bool HasAttribute(int attribute);

    void AddImage(BITMAP *image);
    void AddAttribute(int attribute);
    void SetDimensions(int x_1, int y_1, int x_2, int y_2);
  private:
    int id;
    int x_1, x_2, y_1, y_2;
    std::string name;
    std::string id_str;

    std::vector<int> attributes;
    std::vector<BITMAP*> images;
};

class TileTypeLoader {
  public:
    static void LoadTypes(std::string path);
    static TileType *GetTile(int id);
    static int GetID(std::string id_str);

  private:
    static std::vector<TileType*> types;
};

#endif // TILETYPE_H
