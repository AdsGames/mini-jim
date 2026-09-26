#include "./LightLayer.h"

LightLayer::LightLayer() {
  auto screenSize = asw::display::get_logical_size();
  lightLayer = asw::assets::create_texture(screenSize.x, screenSize.y);
  asw::draw::set_blend_mode(lightLayer, asw::BlendMode::Modulate);

  lightTexture = asw::assets::load_texture("assets/images/spotlight.png");
  asw::draw::set_blend_mode(lightTexture, asw::BlendMode::Add);
}

void LightLayer::setColor(asw::Color color) {
  lightColor = color;
}

void LightLayer::clear() {
  points.clear();
}

void LightLayer::addPoint(const asw::Vec2<float>& point, float level) {
  points.emplace_back(point, level);
}

void LightLayer::draw(const asw::Quad<float>& camera,
                      float destX,
                      float destY) {
  asw::display::set_render_target(lightLayer);
  SDL_SetRenderDrawColor(asw::display::get_renderer(), 64, 64, 64, 0);
  SDL_RenderFillRect(asw::display::get_renderer(), nullptr);

  for (auto& p : points) {
    auto lightSize = 128.0F * p.level;
    const auto lightT =
        asw::Quad<float>(p.position.x - (lightSize / 2),
                         p.position.y - (lightSize / 2), lightSize, lightSize);

    if (!camera.collides(lightT)) {
      continue;
    }

    const auto drawT =
        lightT + asw::Quad<float>(destX - camera.position.x,
                                  destY - camera.position.y, 0, 0);

    asw::draw::stretch_sprite(lightTexture, drawT);
  }

  asw::display::reset_render_target();

  asw::draw::sprite(lightLayer, asw::Vec2<float>(0, 0));
}