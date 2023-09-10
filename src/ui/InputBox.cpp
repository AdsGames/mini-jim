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
  return (signed)asw::input::mouse.x > x &&
         (signed)asw::input::mouse.x < x + width &&
         (signed)asw::input::mouse.y > y &&
         (signed)asw::input::mouse.y < y + height;
}

void InputBox::Update() {
  // Focus
  if (asw::input::mouse.pressed[1]) {
    focused = Hover();

    if (focused) {
      int closest = width;

      for (unsigned int i = 0; i <= text.length(); i++) {
        int textSize = asw::util::getTextSize(font, text.substr(0, i)).x;

        int distance = abs(textSize + x + 6 - (signed)asw::input::mouse.x);

        if (distance < closest) {
          text_iter = i;
          closest = distance;
        }
      }
    }
  }

  int const lastKey = asw::input::keyboard.lastPressed;

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
      if (asw::input::keyboard.down[SDL_SCANCODE_LSHIFT] ||
          asw::input::keyboard.down[SDL_SCANCODE_RSHIFT]) {
        text.insert(text.begin() + text_iter, 'A' - 4 + lastKey);
      } else {
        text.insert(text.begin() + text_iter, 'a' - 4 + lastKey);
      }

      text_iter++;
    }
  }

  // some other, "special" key was pressed; handle it here
  if (asw::input::keyboard.pressed[SDL_SCANCODE_BACKSPACE]) {
    if (text_iter != 0) {
      text_iter--;
      text.erase(text.begin() + text_iter);
    }
  }

  if (asw::input::keyboard.pressed[SDL_SCANCODE_RIGHT]) {
    if (text_iter != text.size()) {
      text_iter++;
    }
  }

  if (asw::input::keyboard.pressed[SDL_SCANCODE_LEFT]) {
    if (text_iter != 0) {
      text_iter--;
    }
  }
}

// Draw box
void InputBox::Draw() const {
  asw::draw::rectFill(x, y, width, height,
                      asw::util::makeColor(12, 12, 12));

  asw::Color const col = (Hover() || focused)
                             ? asw::util::makeColor(230, 230, 230)
                             : asw::util::makeColor(245, 245, 245);

  if (focused) {
    asw::draw::rectFill(x + 2, y + 2, width - 2, height - 2, col);
  } else {
    asw::draw::rectFill(x + 1, y + 1, width - 1, height - 1, col);
  }

  // Output the string to the screen
  asw::draw::text(font, text, x + 6, y, asw::util::makeColor(22, 22, 22));

  // Draw the caret
  if (focused) {
    int textSize = asw::util::getTextSize(font, text.substr(0, text_iter)).x;

    asw::draw::rectFill(textSize + x + 6, y + 8, 7,
                        height - 8, asw::util::makeColor(0, 0, 0));
  }
}
