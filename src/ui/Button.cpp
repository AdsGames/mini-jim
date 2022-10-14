#include "Button.h"

#include "../utility/tools.h"

Button::Button() : Button(0, 0) {}

Button::Button(int x, int y)
    : OnClick(nullptr), x(x), y(y), width(10), height(10) {
  images[0] = nullptr;
  images[1] = nullptr;
}

void Button::SetOnClick(std::function<void(void)> func) {
  OnClick = func;
}

// Load images from file
void Button::SetImages(const char* image1, const char* image2) {
  images[0] = asw::assets::loadTexture(image1);
  images[1] = asw::assets::loadTexture(image2);

  // Size
  auto size = asw::util::getTextureSize(images[0]);
  height = size.y;
  width = size.x;
}

bool Button::Hover() const {
  return collisionAny(asw::input::mouse.x, asw::input::mouse.x, x, x + width,
                      asw::input::mouse.y, asw::input::mouse.y, y, y + height);
}

void Button::Update() {
  if (Hover() && asw::input::mouse.pressed[1] && OnClick != nullptr) {
    OnClick();
  }
}

int Button::GetX() const {
  return x;
}

int Button::GetY() const {
  return y;
}

void Button::Draw() {
  if (images[Hover()]) {
    asw::draw::sprite(images[Hover()], x, y);
  } else {
    asw::draw::rectFill(x, y, x + width, y + height,
                        asw::util::makeColor(60, 60, 60));
  }
}
