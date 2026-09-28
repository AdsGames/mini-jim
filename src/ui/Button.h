#pragma once

#include <asw/asw.h>
#include <array>
#include <functional>

class Button {
 public:
  Button() = default;
  explicit Button(const asw::Vec2f& position);

  void Update();

  void SetImages(const char* image1, const char* image2);

  int GetX() const;
  int GetY() const;

  void SetOnClick(std::function<void()> func);

  // Run the click action, for keyboard or controller selection
  void Activate();

  // In focus mode the button highlights when focused instead of on mouse hover
  void SetFocus(bool focus_mode, bool focused);

  void Draw();

  bool Hover() const;

  // Hovered by the mouse, or focused in focus mode
  bool Highlighted() const;

 private:
  std::function<void(void)> onClick;

  bool focus_mode{false};
  bool focused{false};

  asw::Quadf transform{0, 0, 0, 0};

  std::array<asw::Texture, 2> images;
};
