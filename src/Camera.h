#pragma once

class Camera {
 public:
  Camera();
  Camera(int width, int height, int max_x, int max_y);
  virtual ~Camera() = default;

  void SetSpeed(float speed);
  void Follow(float f_x, float f_y, float dt);
  void SetBounds(int x, int y);

  int GetWidth() const;
  int GetHeight() const;

  int GetX() const;
  int GetY() const;

 private:
  float x;
  float y;

  int width;
  int height;

  int bound_x;
  int bound_y;

  int max_x;
  int max_y;

  float speed;
};
