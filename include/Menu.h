#ifndef MENU_H
#define MENU_H

#include "GameState.h"

#include <allegro.h>
#include <alpng.h>
#include <string>
#include <vector>

#include "fmod/fmod.h"
#include "fmod/fmod_errors.h"

#include "tilemap.h"

#include "globals.h"
#include "tools.h"

class Menu : public GameState
{
  private:
    // Menu/GUI
    BITMAP *buffer, *levelSelectLeft, *levelSelectRight, *levelSelectNumber, *cursor[2], *menuselect, *menu, *help, *copyright, *credits, *menu_player_select, * playerSelector;

    SAMPLE *click, *intro;
    FSOUND_STREAM* music;


    int selectorHovering;
    int step;

    int old_mouse_x;
    int old_mouse_y;

    bool mouse_control;
    bool player_select;

    // Live background
    int animationFrame;
    tileMap *tile_map;
    string scrollDirection;

    // Menu
    int selectorY, selectorX, newSelectorY, selected_object;
    int cursor_x, cursor_y;
    int menu_view_x, menu_view_y;
    bool menuOpen;
  protected:
  public:
    //Main loop functions
    Menu();
    void update();
    void draw();
    ~Menu();
};

#endif // MENU_H
