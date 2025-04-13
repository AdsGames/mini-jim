#pragma once

#include <asw/asw.h>
#include <array>
#include <functional>

class Button {
 public:
  Button() = default;
  explicit Button(const asw::Vec2<float>& position);

  void Update();

  void SetImages(const char* image1, const char* image2);

  int GetX() const;
  int GetY() const;

  void SetOnClick(std::function<void()> func);

  void Draw();

  bool Hover() const;

 private:
  std::function<void(void)> onClick;

  asw::Quad<float> transform{0, 0, 0, 0};

  std::array<asw::Texture, 2> images;
};
