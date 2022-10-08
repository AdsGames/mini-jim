#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <string>
#include "../lib/aar/aar.h"

class InputBox {
 public:
  explicit InputBox(int x = 0,
                    int y = 0,
                    int width = 100,
                    int height = 20,
                    aar::Font* font = nullptr,
                    const std::string& value = "",
                    const std::string& type = "text");

  void Focus();

  std::string GetValue() const;
  void Update();
  void Draw();
  bool Hover() const;

 private:
  int x, y;
  int width, height;

  aar::Font* font;

  std::string text;
  std::string type;

  unsigned int text_iter;
  bool focused;
};

#endif  // INPUTBOX_H
