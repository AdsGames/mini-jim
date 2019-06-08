#include "Camera.h"
#include <cmath>

Camera::Camera() {
  x = 0.0f;
  y = 0.0f;
  height = 0;
  width = 0;
  bound_x = 0;
  bound_y = 0;
  max_x = 0;
  max_y = 0;
  speed = 16.0f;
}

Camera::Camera(int width, int height, int max_x, int max_y) :
  Camera() {
  this -> width = width;
  this -> height = height;
  this -> max_x = max_x;
  this -> max_y = max_y;
  this -> bound_x = width / 2;
  this -> bound_y = height / 2;
}

Camera::~Camera() {}

void Camera::SetSpeed(float speed) {
  this -> speed = speed;
}

void Camera::SetBounds(int x, int y) {
  bound_x = x;
  bound_y = y;
}

void Camera::Follow (float f_x, float f_y) {
  float y_diff = f_y - y;
  float x_diff = f_x - x;

  if (y_diff < bound_y)
    y += (y_diff - bound_y) / speed;
  else if (y_diff > height - bound_y)
    y += (y_diff - (height - bound_y)) / speed;

  if (x_diff < bound_x)
    x += (x_diff - bound_x) / speed;
  else if (x_diff > width - bound_x)
    x += (x_diff - (width - bound_x)) / speed;

  if (y < 0)
    y = 0;
  if (y + height > max_y)
    y = max_y - height;
  if (x < 0)
    x = 0;
  if (x + width > max_x)
    x = max_x - width;
}

int Camera::GetWidth() {
  return width;
}

int Camera::GetHeight() {
  return height;
}

int Camera::GetX() {
  return round(x);
}

int Camera::GetY() {
  return round(y);
}
