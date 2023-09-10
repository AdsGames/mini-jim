#include "Camera.h"
#include <cmath>

Camera::Camera() : Camera(0, 0, 0, 0) {}

Camera::Camera(int width, int height, int max_x, int max_y)
    : x(0),
      y(0),
      width(width),
      height(height),
      bound_x(width / 2),
      bound_y(height / 2),
      max_x(max_x),
      max_y(max_y),
      speed(16.0F) {}

void Camera::SetSpeed(float speed) {
  this->speed = speed;
}

void Camera::SetBounds(int x, int y) {
  bound_x = x;
  bound_y = y;
}

void Camera::Follow(float f_x, float f_y) {
  float const y_diff = f_y - y;
  float const x_diff = f_x - x;

  if (y_diff < bound_y) {
    y += (y_diff - bound_y) / speed;
  } else if (y_diff > height - bound_y) {
    y += (y_diff - (height - bound_y)) / speed;
  }

  if (x_diff < bound_x) {
    x += (x_diff - bound_x) / speed;
  } else if (x_diff > width - bound_x) {
    x += (x_diff - (width - bound_x)) / speed;
  }

  if (y < 0) {
    y = 0;
  }

  if (y + height > max_y) {
    y = max_y - height;
  }

  if (x < 0) {
    x = 0;
  }

  if (x + width > max_x) {
    x = max_x - width;
  }
}

auto Camera::GetWidth() const -> int {
  return width;
}

auto Camera::GetHeight() const -> int {
  return height;
}

auto Camera::GetX() const -> int {
  return std::round(x);
}

auto Camera::GetY() const -> int {
  return std::round(y);
}
