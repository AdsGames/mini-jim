#include "Button.h"

#include <utility>

Button::Button(const asw::Vec2f& position) : onClick(nullptr) {
  transform.position = position;

  images[0] = nullptr;
  images[1] = nullptr;
}

void Button::SetOnClick(std::function<void(void)> func) {
  onClick = std::move(func);
}

// Load images from file
void Button::SetImages(const char* image1, const char* image2) {
  images[0] = asw::assets::load_texture(image1);
  images[1] = asw::assets::load_texture(image2);

  // Size
  transform.size = asw::util::get_texture_size(images[0]);
}

auto Button::Hover() const -> bool {
  return transform.contains(asw::input::get_mouse().position.x,
                            asw::input::get_mouse().position.y);
}

auto Button::Highlighted() const -> bool {
  return focus_mode ? focused : Hover();
}

void Button::SetFocus(bool focus_mode, bool focused) {
  this->focus_mode = focus_mode;
  this->focused = focused;
}

void Button::Activate() {
  if (onClick != nullptr) {
    onClick();
  }
}

void Button::Update() {
  if (Hover() &&
      asw::input::get_mouse_button_down(asw::input::MouseButton::Left) &&
      onClick != nullptr) {
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
  if (images[Highlighted()]) {
    asw::draw::sprite(images[Highlighted()], transform.position);
  } else {
    asw::draw::rect_fill(transform, asw::Color(60, 60, 60));
  }
}
