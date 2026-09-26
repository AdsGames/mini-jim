#pragma once

#include <asw/asw.h>
#include <vector>

enum class LightKind {
  Lamp,
  Fire,
  Heat,
};

struct LightPoint {
  asw::Vec2f position;
  float radius{64.0F};
  LightKind kind{LightKind::Lamp};
};

class LightLayer {
 public:
  LightLayer();

  void clear();

  void addPoint(const asw::Vec2f& point, float radius, LightKind kind);

  // Multiply the view by the light map, halos are player lanterns
  void draw(const asw::Quadf& camera,
            float destX,
            float destY,
            float time_ms,
            const std::vector<asw::Vec2f>& halos);

 private:
  // Add a tinted gradient circle to the current render target
  void drawLight(const asw::Quadf& area,
                 const asw::Color& color,
                 float strength);

  // Low resolution light map, multiplied over the view
  asw::Texture buffer;

  // Soft white circle, drawn additively for each light
  asw::Texture gradient;

  std::vector<LightPoint> points;
};
