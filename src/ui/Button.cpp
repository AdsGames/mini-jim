#include "Button.h"

#include <utility>

#include "../utility/tools.h"

Button::Button(Vec2<int> position) : OnClick(nullptr), position(position) {
  images[0] = nullptr;
  images[1] = nullptr;
}

void Button::SetOnClick(std::function<void(void)> func) {
  OnClick = std::move(func);
}

// Load images from file
void Button::SetImages(const char* image1, const char* image2) {
  images[0] = asw::assets::loadTexture(image1);
  images[1] = asw::assets::loadTexture(image2);

  // Size
  auto texture_size = asw::util::getTextureSize(images[0]);
  size.y = texture_size.y;
  size.x = texture_size.x;
}

auto Button::Hover() const -> bool {
  return collisionAny(asw::input::mouse.x, asw::input::mouse.x, position.x,
                      position.x + size.x, asw::input::mouse.y,
                      asw::input::mouse.y, position.y, position.y + size.y);
}

void Button::Update() {
  if (Hover() && asw::input::mouse.pressed[1] && OnClick != nullptr) {
    OnClick();
  }
}

auto Button::GetX() const -> int {
  return position.x;
}

auto Button::GetY() const -> int {
  return position.y;
}

void Button::Draw() {
  if (images[Hover()]) {
    asw::draw::sprite(images[Hover()], position.x, position.y);
  } else {
    asw::draw::rectFill(position.x, position.y, position.x + size.x,
                        position.y + size.y, asw::util::makeColor(60, 60, 60));
  }
}
