#pragma once

#include <asw/asw.h>
#include <vector>

class LightLayer {
 public:
  LightLayer();

  void setColor(asw::Color color);

  void draw(std::vector<SDL_Point>& points);

 private:
  asw::Texture lightLayer;

  asw::Texture lightTexture;

  asw::Color lightColor{};
};
