#ifndef CAMERA_H
#define CAMERA_H


class Camera {
  public:
    Camera();
    Camera(int width, int height, int max_x, int max_y);
    virtual ~Camera();

    void SetSpeed(int speed);
    void Follow (int x, int y);
    void SetBounds(int x, int y);

    int GetWidth();
    int GetHeight();

    int GetX();
    int GetY();

  private:
    float x, y;
    int width, height;
    int bound_x, bound_y;
    int max_x, max_y;
    int speed;
};

#endif // CAMERA_H
