#ifndef TILE_H
#define TILE_H

#include <allegro.h>
#include <alpng.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>

#include "globals.h"
#include "tools.h"

class tile{
  public:
    tile(int newType);
    ~tile();

    int getX();
    int getY();

    int getWidth();
    int getHeight();

    void setX(int newX);
    void setY(int newY);

    int getType();
    vector<int> getAttribute();
    bool containsAttribute( int newAttribute);

    BITMAP* getImage();

    void setType(int newType);

    void setImages(BITMAP* image1);
    void setImagesAnimated(BITMAP* image1, BITMAP* image2, BITMAP* image3, BITMAP* image4);
    void setImagesAnimatedEight(BITMAP* image1, BITMAP* image2, BITMAP* image3, BITMAP* image4, BITMAP* image5, BITMAP* image6, BITMAP* image7, BITMAP* image8);

    void draw_tile(BITMAP* temp, int xOffset, int yOffset, int newFrame);

  private:
    void setDimensions();
    int x, y;
    int initialX, initialY;
    int width;
    int height;
    int type;
    vector<int> attribute;

    int animated;

    BITMAP* images[8];
};

#endif
