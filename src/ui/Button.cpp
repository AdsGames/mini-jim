#include "ui/Button.h"

#include "utility/tools.h"

Button::Button () {
  images[0] = nullptr;
  images[1] = nullptr;

  height = 10;
  width = 10;

  this -> x = 0;
  this -> y = 0;
}

Button::Button (int x, int y) :
  Button () {

  this -> x = x;
  this -> y = y;
}

Button::~Button() {
  destroy_bitmap (images[0]);
  destroy_bitmap (images[1]);
}

// Load images from file
void Button::SetImages (const char *image1, const char *image2) {
  images[0] = load_png_ex (image1);
  images[1] = load_png_ex (image2);

  // Size
  height = images[0] -> h;
  width = images[0] -> w;
}

bool Button::Hover() {
  return collisionAny(mouse_x, mouse_x, x, x + width, mouse_y, mouse_y, y, y + height);
}

bool Button::Clicked() {
  return Hover() && (mouse_b & 1 || (num_joysticks > 0 && joy[0].button[0].b));
}

int Button::GetX() {
  return x;
}

int Button::GetY() {
  return y;
}

void Button::Draw (BITMAP *buffer) {
  if (images[Hover()]) {
    draw_trans_sprite (buffer, images[Hover()], x, y);
  }
  else {
    rectfill (buffer, x, y, x + width, y + height, 0x999999);
  }
}
