# MiniJim

After being microfied Jim must find a way to become regular size again. Race through levels in singleplayer or multiplayer modes while avoiding new dangers.

[![Reliability Rating](https://sonarcloud.io/api/project_badges/measure?project=AdsGames_mini-jim&metric=reliability_rating)](https://sonarcloud.io/summary/new_code?id=AdsGames_mini-jim)
[![Security Rating](https://sonarcloud.io/api/project_badges/measure?project=AdsGames_mini-jim&metric=security_rating)](https://sonarcloud.io/summary/new_code?id=AdsGames_mini-jim)
[![Maintainability Rating](https://sonarcloud.io/api/project_badges/measure?project=AdsGames_mini-jim&metric=sqale_rating)](https://sonarcloud.io/summary/new_code?id=AdsGames_mini-jim)

## Demo

[Web Demo](https://adsgames.github.io/mini-jim/)

## Setup

### CMake

```bash
cmake --preset debug
cmake --build --preset debug
```

### Build Emscripten

```bash
emcmake cmake --preset debug
cmake --build --preset debug
```

## Level Editor

Using [Tiled](https://www.mapeditor.org/) to create levels. The level files are located in `assets/levels`.
