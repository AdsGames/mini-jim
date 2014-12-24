#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <allegro.h>
#include <alpng.h>

#include "globals.h"
#include "tools.h"

#include "tileMap.h"


class projectile{
  public:
    projectile(int newType, int newX, int newY, int newSpeed);
    ~projectile();

    int getX();
    int getY();

    int getType();
    int getContactFrameCounter();
    bool getContact(tileMap *newMap);

    void setType(int newType);
    void setImage(BITMAP* image1, BITMAP* image2, BITMAP* image3);
    void setSounds( int newType);

    void update();
    void draw(BITMAP* temp, int xOffset, int yOffset);

  private:
    int x;
    int y;
    int speed;
    int type;
    int contact_counter;

    bool contact;

    BITMAP* image[3];
    SAMPLE* sound[2];
};

#endif
