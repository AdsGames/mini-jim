#include "ui/InputBox.h"

#include <iostream>

InputBox::InputBox() {
  this -> x = 0;
  this -> y = 0;
  this -> width = 0;
  this -> height = 0;
  this -> text = "";
  text_iter = 0;
}

InputBox::InputBox(int x, int y, int width, int height, std::string value) {
  this -> x = x;
  this -> y = y;
  this -> width = width;
  this -> height = height;
  this -> text = value;
  text_iter = 0;
}

InputBox::~InputBox() {

}

void InputBox::Focus() {
  focused = true;
}

void InputBox::Unfocus() {
  focused = false;
}

std::string InputBox::GetValue() {
  return text;
}

void InputBox::Update() {
  if (!focused || !keypressed())
    return;

  int  newkey   = readkey();
  char ASCII    = newkey & 0xff;
  char scancode = newkey >> 8;

  // a character key was pressed; add it to the string
  if (ASCII >= 32 && ASCII <= 126 && text.length() < MAX_LENGTH) {
    text.insert (text.begin() + text_iter, ASCII);
    text_iter++;
  }
  // some other, "special" key was pressed; handle it here
  else {
    if (scancode == KEY_BACKSPACE) {
      if (text_iter != 0) {
        text_iter--;
        text.erase (text.begin() + text_iter);
      }
    }

    else if (scancode == KEY_RIGHT) {
      if (text_iter != text.size()) {
        text_iter++;
      }
    }

    else if (scancode == KEY_LEFT) {
      if (text_iter != 0) {
        text_iter--;
      }
    }
  }
}

// Draw box
void InputBox::Draw(BITMAP *buffer) {
  rectfill (buffer, x, y, x + width + 2, y + height + 2, makecol (12, 12, 12));
  rectfill (buffer, x + 2, y + 2, x + width, y + height, makecol (245, 245, 245));

  // Output the string to the screen
  textout_ex (buffer, font, text.c_str(), x + 6, y, makecol (12, 12, 12), -1);

  // Draw the caret
  vline (buffer, text_length(font, text.substr(0, text_iter).c_str()) + x + 3, y + 8, y + height - 8, makecol (0, 0, 0));
}
