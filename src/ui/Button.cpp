#include "Button.h"

#include <utility>

Button::Button(const asw::Vec2<float>& position) : onClick(nullptr) {
  transform.position = position;

  images[0] = nullptr;
  images[1] = nullptr;
}

void Button::SetOnClick(std::function<void(void)> func) {
  onClick = std::move(func);
}

// Load images from file
void Button::SetImages(const char* image1, const char* image2) {
  images[0] = asw::assets::loadTexture(image1);
  images[1] = asw::assets::loadTexture(image2);

  // Size
  transform.size = asw::util::getTextureSize(images[0]);
}

auto Button::Hover() const -> bool {
  return transform.contains(asw::input::mouse.x, asw::input::mouse.y);
}

void Button::Update() {
  if (Hover() && asw::input::mouse.pressed[1] && onClick != nullptr) {
    onClick();
  }
}

auto Button::GetX() const -> int {
  return transform.position.x;
}

auto Button::GetY() const -> int {
  return transform.position.y;
}

void Button::Draw() {
  if (images[Hover()]) {
    asw::draw::sprite(images[Hover()], transform.position);
  } else {
    asw::draw::rectFill(transform, asw::util::makeColor(60, 60, 60));
  }
}
