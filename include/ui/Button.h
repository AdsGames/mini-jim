#ifndef BUTTON_H
#define BUTTON_H

#include <allegro.h>

class Button {
  public:
    Button ();
    Button (int x, int y);
    ~Button();

    bool Hover();
    bool Clicked();

    void SetImages (const char *image1, const char *image2);

    int GetX();
    int GetY();

    void Draw (BITMAP *buffer);

  private:
    int height;
    int width;

    int x;
    int y;

    BITMAP *images[2];
};

#endif

