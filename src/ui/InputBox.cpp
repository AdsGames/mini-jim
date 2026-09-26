#include "InputBox.h"

#include <utility>

InputBox::InputBox(int x,
                   int y,
                   int width,
                   int height,
                   asw::Font font,
                   std::string value,
                   std::string type)
    : x(x),
      y(y),
      width(width),
      height(height),
      font(std::move(font)),
      text(std::move(value)),
      type(std::move(type)) {}

void InputBox::Focus() {
  focused = true;
}

auto InputBox::GetValue() const -> std::string {
  return text;
}

auto InputBox::Hover() const -> bool {
  return (signed)asw::input::get_mouse().position.x > x &&
         (signed)asw::input::get_mouse().position.x < x + width &&
         (signed)asw::input::get_mouse().position.y > y &&
         (signed)asw::input::get_mouse().position.y < y + height;
}

void InputBox::Update() {
  // Focus
  if (asw::input::get_mouse_button_down(asw::input::MouseButton::Left)) {
    focused = Hover();

    if (focused) {
      int closest = width;

      for (unsigned int i = 0; i <= text.length(); i++) {
        int textSize = asw::util::get_text_size(font, text.substr(0, i)).x;

        int distance =
            abs(textSize + x + 6 - (signed)asw::input::get_mouse().position.x);

        if (distance < closest) {
          text_iter = i;
          closest = distance;
        }
      }
    }
  }

  int const lastKey = asw::input::get_keyboard().last_pressed;

  if (!focused || lastKey == -1) {
    return;
  }

  // a character key was pressed; add it to the string
  if (type == "number" || type == "text") {
    // Numeric only
    if (lastKey >= 30 && lastKey <= 38) {
      text.insert(text.begin() + text_iter, lastKey + 19);
      text_iter++;
    }

    if (lastKey == 39) {
      text.insert(text.begin() + text_iter, lastKey + 9);
      text_iter++;
    }
  }

  if (type == "text") {
    if (lastKey >= 4 && lastKey <= 29) {
      if (asw::input::get_key(asw::input::Key::LShift) ||
          asw::input::get_key(asw::input::Key::RShift)) {
        text.insert(text.begin() + text_iter, 'A' - 4 + lastKey);
      } else {
        text.insert(text.begin() + text_iter, 'a' - 4 + lastKey);
      }

      text_iter++;
    }
  }

  // some other, "special" key was pressed; handle it here
  if (asw::input::get_key_down(asw::input::Key::Backspace)) {
    if (text_iter != 0) {
      text_iter--;
      text.erase(text.begin() + text_iter);
    }
  }

  if (asw::input::get_key_down(asw::input::Key::Right)) {
    if (text_iter != text.size()) {
      text_iter++;
    }
  }

  if (asw::input::get_key_down(asw::input::Key::Left)) {
    if (text_iter != 0) {
      text_iter--;
    }
  }
}

// Draw box
void InputBox::Draw() const {
  asw::draw::rect_fill(asw::Quadf(x, y, width, height), asw::Color(12, 12, 12));

  asw::Color const col = (Hover() || focused) ? asw::Color(230, 230, 230)
                                              : asw::Color(245, 245, 245);

  if (focused) {
    asw::draw::rect_fill(asw::Quadf(x + 2, y + 2, width - 2, height - 2), col);
  } else {
    asw::draw::rect_fill(asw::Quadf(x + 1, y + 1, width - 1, height - 1), col);
  }

  // Output the string to the screen
  asw::draw::text(font, text, asw::Vec2(x + 6, y), asw::Color(22, 22, 22));

  // Draw the caret
  if (focused) {
    int textSize = asw::util::get_text_size(font, text.substr(0, text_iter)).x;

    asw::draw::rect_fill(asw::Quadf(textSize + x + 6, y + 8, 7, height - 8),
                         asw::Color(0, 0, 0));
  }
}
