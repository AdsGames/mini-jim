#include "./LightLayer.h"

LightLayer::LightLayer() {
  auto screenSize = asw::display::getLogicalSize();
  lightLayer = asw::assets::createTexture(screenSize.x, screenSize.y);
  asw::draw::setBlendMode(lightLayer, asw::BlendMode::MODULATE);

  lightTexture = asw::assets::loadTexture("assets/images/spotlight.png");
  asw::draw::setBlendMode(lightTexture, asw::BlendMode::ADD);
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
  asw::display::setRenderTarget(lightLayer);
  SDL_SetRenderDrawColor(asw::display::renderer, 64, 64, 64, 0);
  SDL_RenderFillRect(asw::display::renderer, nullptr);

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

    asw::draw::stretchSprite(lightTexture, drawT);
  }

  asw::display::resetRenderTarget();

  asw::draw::sprite(lightLayer, asw::Vec2<float>(0, 0));
}