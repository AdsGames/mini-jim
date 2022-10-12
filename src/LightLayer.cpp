#include "./LightLayer.h"

LightLayer::LightLayer() {
  auto screenSize = asw::display::getLogicalSize();
  lightLayer =
      SDL_CreateTexture(asw::display::renderer, SDL_PIXELFORMAT_RGBA8888,
                        SDL_TEXTUREACCESS_TARGET, screenSize.x, screenSize.y);
  SDL_SetTextureBlendMode(lightLayer, SDL_BLENDMODE_MOD);

  lightTexture = asw::load::texture("assets/images/spotlight.png");
  SDL_SetTextureBlendMode(lightTexture.get(), SDL_BLENDMODE_ADD);
}

void LightLayer::setColor(asw::Color color) {
  lightColor = color;
}

void LightLayer::draw(std::vector<SDL_Point>& points) {
  auto lightSize = asw::util::getTextureSize(lightTexture);

  SDL_SetRenderTarget(asw::display::renderer, lightLayer);
  SDL_SetRenderDrawColor(asw::display::renderer, 64, 64, 64, 0);
  SDL_RenderFillRect(asw::display::renderer, nullptr);

  for (auto t : points) {
    asw::draw::stretchSprite(lightTexture, t.x - lightSize.x / 2,
                             t.y - lightSize.y / 2, lightSize.x, lightSize.y);
  }

  SDL_SetRenderTarget(asw::display::renderer, nullptr);

  SDL_RenderCopy(asw::display::renderer, lightLayer, nullptr, nullptr);
}