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

void LightLayer::draw(std::vector<SDL_Point>& points) {
  auto lightSize = asw::util::getTextureSize(lightTexture);

  asw::display::setRenderTarget(lightLayer);
  SDL_SetRenderDrawColor(asw::display::renderer, 64, 64, 64, 0);
  SDL_RenderFillRect(asw::display::renderer, nullptr);

  for (auto t : points) {
    asw::draw::stretchSprite(
        lightTexture,
        asw::Quad<float>(t.x - lightSize.x / 2, t.y - lightSize.y / 2,
                         lightSize.x, lightSize.y));
  }

  asw::display::resetRenderTarget();

  asw::draw::sprite(lightLayer, asw::Vec2<float>(0, 0));
}