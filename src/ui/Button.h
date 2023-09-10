#ifndef BUTTON_H
#define BUTTON_H

#include <asw/asw.h>
#include <functional>
#include "../utility/Vec2.h"

class Button {
 public:
  Button() = default;
  explicit Button(Vec2<int> position);

  void Update();

  void SetImages(const char* image1, const char* image2);

  auto GetX() const -> int;
  auto GetY() const -> int;

  void SetOnClick(std::function<void()> func);

  void Draw();

  auto Hover() const -> bool;

 private:
  std::function<void(void)> OnClick;

  Vec2<int> position{0, 0};
  Vec2<int> size{10, 10};

  asw::Texture images[2];
};

#endif
