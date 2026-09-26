#include "./LightLayer.h"

#include <algorithm>
#include <cmath>

namespace {
// Light map is this many times smaller than the screen
constexpr float LIGHT_SCALE = 4.0F;

// Light gradient texture size in pixels
constexpr int GRADIENT_SIZE = 128;

// Ambient light, the view is multiplied by it
const asw::Color AMBIENT_LIGHT(52, 56, 88);

// Lights, added to the ambient light
const asw::Color LAMP_LIGHT(255, 235, 190);
const asw::Color FIRE_LIGHT(255, 130, 40);
const asw::Color HEAT_LIGHT(255, 80, 40);
const asw::Color HALO_LIGHT(255, 225, 180);

// Light strengths
constexpr float LAMP_STRENGTH = 0.85F;
constexpr float FIRE_STRENGTH = 0.9F;
constexpr float HEAT_STRENGTH = 0.8F;
constexpr float HALO_STRENGTH = 0.7F;

// Player halo radius in pixels
constexpr float HALO_RADIUS = 256.0F;

// Share of fire and heat light also added straight onto the screen
constexpr float GLOW_STRENGTH = 0.35F;

// Glow radius as a share of the light radius
constexpr float GLOW_RADIUS = 0.6F;

const asw::Color& lightColor(LightKind kind) {
  switch (kind) {
    case LightKind::Fire:
      return FIRE_LIGHT;
    case LightKind::Heat:
      return HEAT_LIGHT;
    default:
      return LAMP_LIGHT;
  }
}

float lightStrength(LightKind kind) {
  switch (kind) {
    case LightKind::Fire:
      return FIRE_STRENGTH;
    case LightKind::Heat:
      return HEAT_STRENGTH;
    default:
      return LAMP_STRENGTH;
  }
}

// -1 to 1, each light out of step with the others
float flicker(const LightPoint& p, float seconds) {
  const float x = p.position.x / 64.0F;
  const float y = p.position.y / 64.0F;

  switch (p.kind) {
    case LightKind::Fire:
      return (std::sin((seconds * 9.0F) + (x * 1.7F)) * 0.6F) +
             (std::sin((seconds * 23.0F) + (y * 2.3F)) * 0.4F);
    case LightKind::Heat:
      return std::sin((seconds * 2.0F) + (x * 0.9F) + (y * 1.3F));
    default:
      return 0.0F;
  }
}

asw::Quadf circle(const asw::Vec2f& centre, float radius) {
  return asw::Quadf(centre.x - radius, centre.y - radius, radius * 2.0F,
                    radius * 2.0F);
}
}  // namespace

LightLayer::LightLayer() {
  auto screenSize = asw::display::get_logical_size();
  buffer = asw::assets::create_texture(
      static_cast<int>(static_cast<float>(screenSize.x) / LIGHT_SCALE),
      static_cast<int>(static_cast<float>(screenSize.y) / LIGHT_SCALE));
  asw::draw::set_blend_mode(buffer, asw::BlendMode::Modulate);
  asw::draw::set_scale_mode(buffer, asw::ScaleMode::Linear);

  gradient = asw::assets::create_radial_gradient(
      GRADIENT_SIZE, asw::Color(255, 255, 255, 255),
      asw::Color(255, 255, 255, 0));
  asw::draw::set_blend_mode(gradient, asw::BlendMode::Add);
  asw::draw::set_scale_mode(gradient, asw::ScaleMode::Linear);
}

void LightLayer::clear() {
  points.clear();
}

void LightLayer::addPoint(const asw::Vec2f& point,
                          float radius,
                          LightKind kind) {
  points.push_back(LightPoint{point, radius, kind});
}

void LightLayer::drawLight(const asw::Quadf& area,
                           const asw::Color& color,
                           float strength) {
  asw::draw::set_tint(gradient, color);
  asw::draw::set_alpha(gradient, std::clamp(strength, 0.0F, 1.0F));
  asw::draw::stretch_sprite(gradient, area);
}

void LightLayer::draw(const asw::Quadf& camera,
                      float destX,
                      float destY,
                      float time_ms,
                      const std::vector<asw::Vec2f>& halos) {
  const float seconds = time_ms / 1000.0F;

  // World pixels to screen pixels
  const asw::Vec2f offset(destX - camera.position.x,
                          destY - camera.position.y);

  // Light map: ambient colour, then lights added on top
  asw::display::set_render_target(buffer);
  asw::display::clear(AMBIENT_LIGHT);

  for (const auto& p : points) {
    const float f = flicker(p, seconds);
    const float radius = p.radius * (1.0F + (0.04F * f));

    if (!camera.collides(circle(p.position, radius))) {
      continue;
    }

    drawLight(circle(p.position + offset, radius) / LIGHT_SCALE,
              lightColor(p.kind),
              lightStrength(p.kind) * (0.92F + (0.08F * f)));
  }

  // Players carry a small lantern
  for (const auto& h : halos) {
    if (!camera.collides(circle(h, HALO_RADIUS))) {
      continue;
    }

    drawLight(circle(h + offset, HALO_RADIUS) / LIGHT_SCALE, HALO_LIGHT,
              HALO_STRENGTH);
  }

  asw::display::reset_render_target();

  // Multiply the view by the light map
  auto screenSize = asw::display::get_logical_size();
  asw::draw::stretch_sprite(
      buffer, asw::Quadf(0, 0, static_cast<float>(screenSize.x),
                         static_cast<float>(screenSize.y)));

  // Fire and heat also glow, so they look hot and not only less dark
  for (const auto& p : points) {
    if (p.kind == LightKind::Lamp) {
      continue;
    }

    const float f = flicker(p, seconds);
    const float radius = p.radius * GLOW_RADIUS * (1.0F + (0.06F * f));

    if (!camera.collides(circle(p.position, radius))) {
      continue;
    }

    drawLight(circle(p.position + offset, radius), lightColor(p.kind),
              GLOW_STRENGTH * (0.85F + (0.15F * f)));
  }

  asw::draw::set_tint(gradient, asw::color::white);
  asw::draw::set_alpha(gradient, 1.0F);
}
