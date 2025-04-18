#include "Camera.h"
#include <cmath>

Camera::Camera() : Camera(0, 0, 0, 0) {}

Camera::Camera(float width, float height, float max_x, float max_y)
    : viewport(0, 0, width, height),
      bounds(width / 2, height / 2),
      max_pos(max_x, max_y),
      speed(16.0F) {}

void Camera::setSpeed(float speed) {
  this->speed = speed;
}

void Camera::setBounds(float x, float y) {
  bounds.x = x;
  bounds.y = y;
}

void Camera::follow(const asw::Vec2<float>& pos, float dt) {
  const auto diff = pos - viewport.position;
  const float speed_dt = (speed / 16.0F) * dt;

  if (diff.y < bounds.y) {
    viewport.position.y += (diff.y - bounds.y) / speed_dt;
  } else if (diff.y > viewport.size.y - bounds.y) {
    viewport.position.y += (diff.y - (viewport.size.y - bounds.y)) / speed_dt;
  }

  if (diff.x < bounds.x) {
    viewport.position.x += (diff.x - bounds.x) / speed_dt;
  } else if (diff.x > viewport.size.x - bounds.x) {
    viewport.position.x += (diff.x - (viewport.size.x - bounds.x)) / speed_dt;
  }

  if (viewport.position.y < 0) {
    viewport.position.y = 0;
  }

  if (viewport.position.y + viewport.size.y > max_pos.y) {
    viewport.position.y = max_pos.y - viewport.size.y;
  }

  if (viewport.position.x < 0) {
    viewport.position.x = 0;
  }

  if (viewport.position.x + viewport.size.x > max_pos.x) {
    viewport.position.x = max_pos.x - viewport.size.x;
  }
}
