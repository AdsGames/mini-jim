#pragma once

#include <asw/asw.h>
#include <vector>

class LightPoint {
 public:
  LightPoint(const asw::Vec2<float>& position, int level = 1)
      : position(position), level(level) {}

  asw::Vec2<float> position;
  float level{1.0F};
};

class LightLayer {
 public:
  LightLayer();

  void setColor(asw::Color color);

  void draw(const asw::Quad<float>& camera, float destX, float destY);

  void clear();

  void addPoint(const asw::Vec2<float>& point, float level = 1.0F);

 private:
  asw::Texture lightLayer;

  asw::Texture lightTexture;

  asw::Color lightColor{};

  std::vector<LightPoint> points;
};
