# MiniJim
After being microfied Jim must find a way to become regular size again. Race through levels in singleplayer or multiplayer modes while avoiding new dangers.

## Compiling

### Windows
You must install allegro 4, lib ogg and the allegro loadpng addon:
- [Allegro 4](http://liballeg.org/api.html)


Open code blocks project, select debug as the build target and build


### Linux
```sudo apt-get install liballegro4-dev```

```sudo apt-get install libloadpng4-dev```

```sudo apt-get install liblogg4-dev```

Open code blocks project, select debug-linux as the build target and build

## Linking
In case you are not using the code blocks project provided
```
-llogg
-lloadpng
-lalleg44 or -lalleg
```
