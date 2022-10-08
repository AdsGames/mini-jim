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

/**
 * @brief Display
 *
 */
namespace display {

// Renderer
extern aar::Renderer* renderer;
extern aar::Window* window;

inline void setTitle(const std::string& title) {
  SDL_SetWindowTitle(aar::display::window, title.c_str());
}

inline void setIcon(const std::string& path) {
  SDL_Surface* icon = IMG_Load(path.c_str());

  if (!icon) {
    return;
  }

  SDL_SetWindowIcon(aar::display::window, icon);
}

inline void setFullscreen(bool fullscreen) {
  if (fullscreen) {
    SDL_SetWindowFullscreen(aar::display::window, SDL_WINDOW_FULLSCREEN);
  } else {
    SDL_SetWindowFullscreen(aar::display::window, 0);
  }
}

inline void setResolution(int w, int h) {
  SDL_SetWindowSize(aar::display::window, w, h);
}

inline SDL_Point getSize() {
  SDL_Point size;
  SDL_GetWindowSize(aar::display::window, &size.x, &size.y);
  return size;
}

inline SDL_Point getLogicalSize() {
  SDL_Point size;
  SDL_RenderGetLogicalSize(aar::display::renderer, &size.x, &size.y);
  return size;
}

inline SDL_FPoint getScale() {
  SDL_FPoint scale;
  SDL_RenderGetScale(aar::display::renderer, &scale.x, &scale.y);
  return scale;
}

}  // namespace display

namespace core {

extern bool exit;

inline void update() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
      case SDL_WINDOWEVENT:
        switch (e.window.event) {
          case SDL_WINDOWEVENT_RESIZED: {
            // Maintain aspect ratio
            SDL_FPoint scale;
            SDL_RenderGetScale(aar::display::renderer, &scale.x, &scale.y);

            SDL_Point size;
            SDL_RenderGetLogicalSize(aar::display::renderer, &size.x, &size.y);

            SDL_SetWindowSize(aar::display::window, size.x * scale.x,
                              size.y * scale.y);
            break;
          }
        }
        break;

      case SDL_QUIT:
        exit = true;
        break;

      default:
        break;
    }
  }
}
}  // namespace core

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

inline void init(int width = 640,
                 int height = 480,
                 const std::string& windowTitle = "AAR Game") {
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

  aar::display::window = SDL_CreateWindow(
      windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if (!aar::display::window) {
    aar::util::abortOnError("WINDOW");
  }

  // Get window surface
  aar::display::renderer =
      SDL_CreateRenderer(aar::display::window, -1, SDL_RENDERER_ACCELERATED);

  SDL_RenderSetLogicalSize(aar::display::renderer, width, height);

  SDL_SetRenderDrawBlendMode(aar::display::renderer, SDL_BLENDMODE_BLEND);
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

inline SDL_Point getTextSize(Font* font, const std::string& text) {
  SDL_Point size;
  TTF_SizeText(font, text.c_str(), &size.x, &size.y);
  return size;
}

}  // namespace util

/**
 * @brief Draw routines
 *
 */
namespace draw {

inline void clearColor(aar::Color color) {
  SDL_SetRenderDrawColor(aar::display::renderer, color.r, color.g, color.b,
                         color.a);
  SDL_RenderClear(aar::display::renderer);
}

inline void sprite(aar::Texture* bmp, int x, int y) {
  SDL_Point size = aar::util::getTextureSize(bmp);
  SDL_Rect dest = {x, y, size.x, size.y};
  SDL_RenderCopy(aar::display::renderer, bmp, nullptr, &dest);
}

inline void spriteHFlip(aar::Texture* bmp, int x, int y) {
  SDL_Point size = aar::util::getTextureSize(bmp);
  SDL_Rect dest = {x, y, size.x, size.y};
  SDL_RenderCopy(aar::display::renderer, bmp, nullptr, &dest);
}

inline void spriteVFlip(aar::Texture* bmp, int x, int y) {
  SDL_Point size = aar::util::getTextureSize(bmp);
  SDL_Rect dest = {x, y, size.x, size.y};
  SDL_RenderCopy(aar::display::renderer, bmp, nullptr, &dest);
}

inline void spriteVHFlip(aar::Texture* bmp, int x, int y) {
  SDL_Point size = aar::util::getTextureSize(bmp);
  SDL_Rect dest = {x, y, size.x, size.y};
  SDL_RenderCopy(aar::display::renderer, bmp, nullptr, &dest);
}

inline void stretchSprite(aar::Texture* bmp, int x, int y, int w, int h) {
  SDL_Rect dest = {x, y, w, h};
  SDL_RenderCopy(aar::display::renderer, bmp, nullptr, &dest);
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
  SDL_RenderCopy(aar::display::renderer, bmp, &src, &dest);
}

inline void text(aar::Font* font,
                 std::string text,
                 int x,
                 int y,
                 aar::Color color) {
  SDL_Color sdlColor = {color.r, color.g, color.b, color.a};
  SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), sdlColor);
  SDL_Texture* textTexture =
      SDL_CreateTextureFromSurface(aar::display::renderer, textSurface);
  SDL_Rect dest = {x, y, textSurface->w, textSurface->h};
  SDL_RenderCopy(aar::display::renderer, textTexture, nullptr, &dest);
  SDL_FreeSurface(textSurface);
  SDL_DestroyTexture(textTexture);
}

inline void textCenter(aar::Font* font,
                       std::string text,
                       int x,
                       int y,
                       aar::Color color) {
  auto size = aar::util::getTextSize(font, text);
  aar::draw::text(font, text, x - size.x / 2, y, color);
}

inline void primRectFill(int x1, int y1, int x2, int y2, aar::Color color) {
  SDL_Rect rect = {x1, y1, x2 - x1, y2 - y1};
  SDL_SetRenderDrawColor(aar::display::renderer, color.r, color.g, color.b,
                         color.a);
  SDL_RenderFillRect(aar::display::renderer, &rect);
}

inline void primRect(int x1, int y1, int x2, int y2, aar::Color color) {
  SDL_Rect rect = {x1, y1, x2 - x1, y2 - y1};
  SDL_SetRenderDrawColor(aar::display::renderer, color.r, color.g, color.b,
                         color.a);
  SDL_RenderDrawRect(aar::display::renderer, &rect);
}

}  // namespace draw

/**
 * @brief Load routines
 *
 */
namespace load {

inline aar::Texture* bitmap(const std::string& filename) {
  SDL_Texture* text = IMG_LoadTexture(aar::display::renderer, filename.c_str());

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

inline void destroyTexture(aar::Texture* bitmap) {
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
  SDL_Texture* text =
      SDL_CreateTextureFromSurface(aar::display::renderer, temp);
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
                 int loop = 0) {
  int channel = Mix_GroupAvailable(0);
  Mix_VolumeChunk(sample, volume);
  Mix_SetPanning(channel, pan, 255 - pan);
  Mix_PlayChannel(-1, sample, loop);
}

}  // namespace sound

}  // namespace aar

#endif  // AAR_H