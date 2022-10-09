#include "InputBox.h"

#include <iostream>

#include <asw/util/KeyListener.h>
#include <asw/util/MouseListener.h>

InputBox::InputBox(int x,
                   int y,
                   int width,
                   int height,
                   asw::Font font,
                   const std::string& value,
                   const std::string& type)
    : x(x),
      y(y),
      width(width),
      height(height),
      font(font),
      text(value),
      type(type),
      text_iter(0),
      focused(false) {}

void InputBox::Focus() {
  focused = true;
}

std::string InputBox::GetValue() const {
  return text;
}

bool InputBox::Hover() const {
  return (signed)MouseListener::x > x && (signed)MouseListener::x < x + width &&
         (signed)MouseListener::y > y && (signed)MouseListener::y < y + height;
}

void InputBox::Update() {
  // Focus
  if (MouseListener::mouse_pressed & 1) {
    focused = Hover();

    if (focused) {
      int closest = width;

      for (unsigned int i = 0; i <= text.length(); i++) {
        int textSize = asw::util::getTextSize(font, text.substr(0, i)).x;

        int distance = abs(textSize + x + 6 - (signed)MouseListener::x);

        if (distance < closest) {
          text_iter = i;
          closest = distance;
        }
      }
    }
  }

  int lastKey = KeyListener::lastKeyPressed;

  if (!focused || lastKey == -1) {
    return;
  }

  std::cout << "Key: " << lastKey << std::endl;

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
      if (KeyListener::keyDown[SDL_SCANCODE_LSHIFT] ||
          KeyListener::keyDown[SDL_SCANCODE_RSHIFT]) {
        text.insert(text.begin() + text_iter, 'A' - 4 + lastKey);
      } else {
        text.insert(text.begin() + text_iter, 'a' - 4 + lastKey);
      }

      text_iter++;
    }
  }

  // some other, "special" key was pressed; handle it here
  if (KeyListener::keyPressed[SDL_SCANCODE_BACKSPACE]) {
    if (text_iter != 0) {
      text_iter--;
      text.erase(text.begin() + text_iter);
    }
  }

  if (KeyListener::keyPressed[SDL_SCANCODE_RIGHT]) {
    if (text_iter != text.size()) {
      text_iter++;
    }
  }

  if (KeyListener::keyPressed[SDL_SCANCODE_LEFT]) {
    if (text_iter != 0) {
      text_iter--;
    }
  }
}

// Draw box
void InputBox::Draw() {
  asw::draw::primRectFill(x, y, x + width, y + height,
                          asw::util::makeColor(12, 12, 12));

  asw::Color col = (Hover() || focused) ? asw::util::makeColor(230, 230, 230)
                                        : asw::util::makeColor(245, 245, 245);

  if (focused)
    asw::draw::primRectFill(x + 2, y + 2, x + width - 2, y + height - 2, col);
  else
    asw::draw::primRectFill(x + 1, y + 1, x + width - 1, y + height - 1, col);

  // Output the string to the screen
  asw::draw::text(font, text.c_str(), x + 6, y,
                  asw::util::makeColor(22, 22, 22));

  // Draw the caret
  if (focused) {
    int textSize = asw::util::getTextSize(font, text.substr(0, text_iter)).x;

    asw::draw::primRectFill(textSize + x + 6, y + 8, textSize + x + 7,
                            y + height - 8, asw::util::makeColor(0, 0, 0));
  }
}
