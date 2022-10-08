#ifndef BUTTON_H
#define BUTTON_H

#include <functional>
#include "../lib/aar/aar.h"

class Button {
 public:
  Button();
  Button(int x, int y);
  ~Button();

  void Update();

  void SetImages(const char* image1, const char* image2);

  int GetX() const;
  int GetY() const;

  void SetOnClick(std::function<void()> func);

  void Draw();

  bool Hover() const;

 private:
  std::function<void(void)> OnClick;

  int x, y;
  int width, height;

  aar::Texture* images[2];
};

#endif
