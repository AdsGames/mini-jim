#pragma once

#include <asw/asw.h>

class Camera {
 public:
  Camera();
  Camera(float width, float height, float max_x, float max_y);

  void setSpeed(float speed);
  void follow(const asw::Vec2<float>& pos, float dt);
  void setBounds(float x, float y);

  const asw::Quad<float>& getViewport() const { return viewport; }

 private:
  asw::Quad<float> viewport;

  asw::Vec2<float> bounds;

  asw::Vec2<float> max_pos;

  float speed;
};
