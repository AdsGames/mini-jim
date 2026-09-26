#pragma once

#include <asw/asw.h>

class Camera {
 public:
  Camera();
  Camera(float width, float height, float max_x, float max_y);

  void setSpeed(float speed);
  void follow(const asw::Vec2& pos, float dt);
  void setBounds(float x, float y);

  const asw::Quadf& getViewport() const { return viewport; }

 private:
  asw::Quadf viewport;

  asw::Vec2 bounds;

  asw::Vec2 max_pos;

  float speed;
};
