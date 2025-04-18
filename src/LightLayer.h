#pragma once

#include <asw/asw.h>
#include <vector>

class LightLayer {
 public:
  LightLayer();

  void setColor(asw::Color color);

  void draw(const std::vector<asw::Vec2<float>>& points);

 private:
  asw::Texture lightLayer;

  asw::Texture lightTexture;

  asw::Color lightColor{};
};
