#ifndef CAMERA_H
#define CAMERA_H

class Camera {
 public:
  Camera();
  Camera(int width, int height, int max_x, int max_y);
  virtual ~Camera() = default;

  void SetSpeed(float speed);
  void Follow(float f_x, float f_y);
  void SetBounds(int x, int y);

  int GetWidth() const;
  int GetHeight() const;

  int GetX() const;
  int GetY() const;

 private:
  float x, y;
  int width, height;
  int bound_x, bound_y;
  int max_x, max_y;
  float speed;
};

#endif  // CAMERA_H
