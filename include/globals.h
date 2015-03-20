#ifndef GLOBALS_H
#define GLOBALS_H

//All libaries
#include <allegro.h>

//All defined presets put here
//tile Attributes
#define gas 0
#define solid 1
#define liquid 2
#define climb 3
#define harmful 4
#define interactive 5
#define item 6
#define spawn 7
#define half_block_top 8
#define half_block_bottom 9
#define quarter_block_top 10
#define quarter_block_bottom 11
#define light 12
#define slide 13

//Character directions
// 0-3 left, 4-7 right, 8-11 up
#define LEFT 0
#define RIGHT 4
#define UP 8

//Tiles
#define tile_air 0
#define tile_checkpoint 1
#define tile_oven_fill 2
#define tile_oven_top 3
#define tile_brick_red 4
#define tile_tack 7
#define tile_carpet 8
#define tile_carpet_top 9
#define tile_cement 10
#define tile_dice 11
#define tile_tile 12

#define tile_book 13
#define tile_finish 15

#define tile_water 16
#define tile_water_deep 17
#define tile_bottle 18
#define tile_wood 19
#define tile_stained_glass 20
#define tile_white_tile 21
#define tile_blue_wallpaper 22
#define tile_blue_wallpaper_bottom 23
#define tile_faucet 24
#define tile_mousetrap_1 25
#define tile_mousetrap_2 26

#define tile_toaster_block 27
#define tile_toaster_left 28
#define tile_toaster_right 29
#define tile_toaster_center 30
#define tile_toaster_element 31

#define tile_chicken_still 34
#define tile_chicken 35
#define tile_beak 36

#define tile_glasses 37
#define tile_plate 38
#define tile_plate_stack 39
#define tile_blade 40

#define tile_fire 41

#define tile_claw 42

#define tile_claw_extension 43
#define tile_bed1 44
#define tile_bed2 45
#define tile_pillow 46
#define tile_car 47
#define tile_coca 48
#define tile_dirt 49
#define tile_grass 50
#define tile_element 51
#define tile_red_cloth 52
#define tile_wood_light 53
#define tile_pipe1 56
#define tile_pipe2 57
#define tile_fork1 58
#define tile_fork2 59
#define tile_bed3 60
#define tile_ovenright 61
#define tile_ovenleft 62
#define tile_ovencenter 63
#define tile_light 64
#define tile_cement_full 65
#define tile_wall 66
#define tile_wall_slide 67

//Background Tiles
#define tile_back_grey 100

#define tile_back_grey_shadow_top 101
#define tile_back_grey_shadow_top_right 102
#define tile_back_grey_shadow_top_left 103
#define tile_back_grey_shadow_left 104
#define tile_back_grey_shadow_bottom_left 105
#define tile_back_grey_shadow_top_left_corner 106

#define tile_back_grey_dark 107
#define tile_back_grey_dark_top_left 108
#define tile_back_grey_dark_top_right 109

#define tile_back_lamp 110
#define tile_wood_light_slide 111

//Spawners
#define tile_spawn_player 199

//Enemies
#define enemy_vorticon 0

extern BITMAP* tileSprites[400][8];
extern BITMAP* projectileSprites[10][3];

// Set next state
void set_next_state( int newState );

//Game states
enum GameStates
{
    STATE_NULL,
    STATE_INIT,
    STATE_INTRO,
    STATE_MENU,
    STATE_EDIT,
    STATE_GAME,
    STATE_EXIT,
};

// Frames
extern int frames_done;

// Resdiv
extern int resDiv;
extern bool single_player;

//State variables
extern int stateID;
extern int nextState;
extern int levelOn;

#endif
