#ifndef AAR_H
#define AAR_H

#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <string>

namespace aar {

typedef SDL_Texture Texture;
typedef TTF_Font Font;
typedef Mix_Chunk Sample;
typedef SDL_Renderer Renderer;
typedef SDL_Color Color;
typedef SDL_Window Window;

// Renderer
extern aar::Renderer* renderer;

/**
 * @brief Utility routines
 *
 */
namespace util {

inline void abortOnError(const std::string& message) {
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", message.c_str(),
                           nullptr);
  exit(-1);
}

inline void init() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
    abortOnError("SDL_Init");
  }

  // Initialize PNG loading
  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags)) {
    abortOnError("IMG_Init");
  }

  if (TTF_Init()) {
    abortOnError("TTF_Init");
  }

  // Initialize SDL_mixer
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    abortOnError("Mix_OpenAudio");
  }
}

inline Color makeColor(int r, int g, int b) {
  Color color;
  color.r = r;
  color.g = g;
  color.b = b;
  color.a = 255;
  return color;
}

inline Color makeColor(int r, int g, int b, int a) {
  Color color;
  color.r = r;
  color.g = g;
  color.b = b;
  color.a = a;
  return color;
}

inline SDL_Point getTextureSize(Texture* texture) {
  SDL_Point size;
  SDL_QueryTexture(texture, nullptr, nullptr, &size.x, &size.y);
  return size;
}

}  // namespace util

/**
 * @brief Draw routines
 *
 */
namespace draw {

inline void clearColor(aar::Color color) {
  SDL_SetRenderDrawColor(aar::renderer, color.r, color.g, color.b, color.a);
  SDL_RenderClear(aar::renderer);
}

inline void sprite(aar::Texture* bmp, int x, int y) {
  SDL_Point size = aar::util::getTextureSize(bmp);
  SDL_Rect dest = {x, y, size.x, size.y};
  SDL_RenderCopy(aar::renderer, bmp, nullptr, &dest);
}

inline void spriteHFlip(aar::Texture* bmp, int x, int y) {
  SDL_Point size = aar::util::getTextureSize(bmp);
  SDL_Rect dest = {x, y, size.x, size.y};
  SDL_RenderCopy(aar::renderer, bmp, nullptr, &dest);
}

inline void spriteVFlip(aar::Texture* bmp, int x, int y) {
  SDL_Point size = aar::util::getTextureSize(bmp);
  SDL_Rect dest = {x, y, size.x, size.y};
  SDL_RenderCopy(aar::renderer, bmp, nullptr, &dest);
}

inline void spriteVHFlip(aar::Texture* bmp, int x, int y) {
  SDL_Point size = aar::util::getTextureSize(bmp);
  SDL_Rect dest = {x, y, size.x, size.y};
  SDL_RenderCopy(aar::renderer, bmp, nullptr, &dest);
}

inline void stretchSprite(aar::Texture* bmp, int x, int y, int w, int h) {
  SDL_Rect dest = {x, y, w, h};
  SDL_RenderCopy(aar::renderer, bmp, nullptr, &dest);
}

inline void stretchSpriteBlit(aar::Texture* bmp,
                              int x1,
                              int y1,
                              int w1,
                              int h1,
                              int x2,
                              int y2,
                              int w2,
                              int h2) {
  const SDL_Rect src = {x1, y1, w1, h1};
  SDL_Rect dest = {x2, y2, w2, h2};
  SDL_RenderCopy(aar::renderer, bmp, &src, &dest);
}

inline void text(aar::Font* font,
                 std::string text,
                 int x,
                 int y,
                 aar::Color color) {
  SDL_Color sdlColor = {color.r, color.g, color.b, color.a};
  SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), sdlColor);
  SDL_Texture* textTexture =
      SDL_CreateTextureFromSurface(aar::renderer, textSurface);

  SDL_Rect dest = {x, y, textSurface->w, textSurface->h};
  SDL_RenderCopy(aar::renderer, textTexture, nullptr, &dest);
  SDL_FreeSurface(textSurface);
}

inline void primRectFill(int x1, int y1, int x2, int y2, aar::Color color) {
  SDL_Rect rect = {x1, y1, x2 - x1, y2 - y1};
  SDL_SetRenderDrawColor(aar::renderer, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(aar::renderer, &rect);
}

inline void primRect(int x1, int y1, int x2, int y2, aar::Color color) {
  SDL_Rect rect = {x1, y1, x2 - x1, y2 - y1};
  SDL_SetRenderDrawColor(aar::renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawRect(aar::renderer, &rect);
}

}  // namespace draw

/**
 * @brief Load routines
 *
 */
namespace load {

inline aar::Texture* bitmap(const std::string& filename) {
  SDL_Texture* text = IMG_LoadTexture(aar::renderer, filename.c_str());

  if (!text) {
    util::abortOnError("Failed to load bitmap: " + filename);
  }

  SDL_SetTextureBlendMode(text, SDL_BLENDMODE_BLEND);

  return text;
}

inline aar::Font* font(const std::string& filename, int size) {
  aar::Font* temp = TTF_OpenFont(filename.c_str(), size);

  if (!temp) {
    util::abortOnError("Failed to load font: " + filename);
  }
  return temp;
}

inline aar::Sample* sample(const std::string& filename) {
  aar::Sample* temp = Mix_LoadWAV(filename.c_str());

  if (!temp) {
    util::abortOnError("Failed to load sample: " + filename);
  }
  return temp;
}

inline aar::Sample* sampleOgg(const std::string& filename) {
  aar::Sample* temp = Mix_LoadWAV(filename.c_str());

  if (!temp) {
    util::abortOnError("Failed to load sample: " + filename);
  }
  return temp;
}

inline void destroyBitmap(aar::Texture* bitmap) {
  SDL_DestroyTexture(bitmap);
}

inline void destroyFont(aar::Font* font) {
  TTF_CloseFont(font);
}

inline void destroySample(aar::Sample* sample) {
  Mix_FreeChunk(sample);
}

inline Texture* createBitmap(int w, int h) {
  SDL_Surface* temp = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
  SDL_Texture* text = SDL_CreateTextureFromSurface(aar::renderer, temp);
  return text;
}
}  // namespace load

/**
 * @brief Sound routines
 *
 */
namespace sound {
inline void play(aar::Sample* sample,
                 int volume = 255,
                 int pan = 128,
                 int freq = 1000,
                 int loop = 0) {
  int channel = Mix_GroupAvailable(0);
  Mix_VolumeChunk(sample, volume);
  Mix_SetPanning(channel, pan, 255 - pan);
  Mix_PlayChannel(-1, sample, loop);
}

}  // namespace sound

}  // namespace aar

#endif  // AAR_H