#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <asw/asw.h>
#include <string>

class InputBox {
 public:
  explicit InputBox(int x = 0,
                    int y = 0,
                    int width = 100,
                    int height = 20,
                    asw::Font font = nullptr,
                    std::string value = "",
                    std::string type = "text");

  void Focus();

  auto GetValue() const -> std::string;
  void Update();
  void Draw() const;
  auto Hover() const -> bool;

 private:
  int x, y;
  int width, height;

  asw::Font font;

  std::string text{};
  std::string type{};

  unsigned int text_iter{0};
  bool focused{false};
};

#endif  // INPUTBOX_H
