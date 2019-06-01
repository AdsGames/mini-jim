#include "Init.h"

#include <allegro.h>

#include "globals.h"
#include "utility/tools.h"

Init::Init() {
  set_window_title ("Setting up");

  // Setup number generator
  srand (time (nullptr));

  // Tile Sprites
  // Not animated
  tileSprites[tile_air][0] = load_png_ex ("images/blocks/air.png");
  tileSprites[tile_brick_red][0] = load_png_ex ("images/blocks/brick_red.png");
  tileSprites[tile_finish][0] = load_png_ex ("images/blocks/finish.png");

  tileSprites[tile_checkpoint][0] = load_png_ex ("images/blocks/checkpoint0.png");
  tileSprites[tile_checkpoint][1] = load_png_ex ("images/blocks/checkpoint1.png");
  tileSprites[tile_checkpoint][2] = load_png_ex ("images/blocks/checkpoint2.png");
  tileSprites[tile_checkpoint][3] = load_png_ex ("images/blocks/checkpoint3.png");

  // New tiles
  tileSprites[tile_carpet][0] = load_png_ex ("images/blocks/carpet.png");
  tileSprites[tile_cement][0] = load_png_ex ("images/blocks/cement.png");
  tileSprites[tile_cement_full][0] = load_png_ex ("images/blocks/cement_full.png");
  tileSprites[tile_dice][0] = load_png_ex ("images/blocks/dice.png");
  tileSprites[tile_tile][0] = load_png_ex ("images/blocks/tile.png");
  tileSprites[tile_book][0] = load_png_ex ("images/blocks/book.png");


  tileSprites[tile_carpet_top][0] = load_png_ex ("images/blocks/carpet_top.png");
  tileSprites[tile_water_deep][0] = load_png_ex ("images/blocks/water_deep.png");
  tileSprites[tile_bottle][0] = load_png_ex ("images/blocks/bottle.png");
  tileSprites[tile_wood][0] = load_png_ex ("images/blocks/wood.png");
  tileSprites[tile_stained_glass][0] = load_png_ex ("images/blocks/glass.png");
  tileSprites[tile_white_tile][0] = load_png_ex ("images/blocks/white_tile.png");
  tileSprites[tile_blue_wallpaper][0] = load_png_ex ("images/blocks/blue_wallpaper.png");
  tileSprites[tile_blue_wallpaper_bottom][0] = load_png_ex ("images/blocks/blue_wallpaper_bottom.png");
  tileSprites[tile_faucet][0] = load_png_ex ("images/blocks/faucet.png");
  tileSprites[tile_tack][0] = load_png_ex ("images/blocks/tack.png");
  tileSprites[tile_mousetrap_1][0] = load_png_ex ("images/blocks/mousetrap_1.png");
  tileSprites[tile_mousetrap_2][0] = load_png_ex ("images/blocks/mousetrap_2.png");

  tileSprites[tile_chicken][0] = load_png_ex ("images/blocks/chicken_1.png");
  tileSprites[tile_chicken][1] = load_png_ex ("images/blocks/chicken_4.png");
  tileSprites[tile_chicken][2] = load_png_ex ("images/blocks/chicken_3.png");
  tileSprites[tile_chicken][3] = load_png_ex ("images/blocks/chicken_2.png");
  tileSprites[tile_chicken][4] = load_png_ex ("images/blocks/chicken_1.png");
  tileSprites[tile_chicken][5] = load_png_ex ("images/blocks/chicken_1.png");
  tileSprites[tile_chicken][6] = load_png_ex ("images/blocks/chicken_1.png");
  tileSprites[tile_chicken][7] = load_png_ex ("images/blocks/chicken_1.png");


  tileSprites[tile_claw][0] = load_png_ex ("images/blocks/claw1.png");
  tileSprites[tile_claw][1] = load_png_ex ("images/blocks/claw2.png");
  tileSprites[tile_claw][2] = load_png_ex ("images/blocks/claw3.png");
  tileSprites[tile_claw][3] = load_png_ex ("images/blocks/claw4.png");

  tileSprites[tile_beak][0] = load_png_ex ("images/blocks/beak.png");

  tileSprites[tile_glasses][0] = load_png_ex ("images/blocks/glasses.png");
  tileSprites[tile_plate][0] = load_png_ex ("images/blocks/plate.png");
  tileSprites[tile_plate_stack][0] = load_png_ex ("images/blocks/plate_stack.png");
  tileSprites[tile_blade][0] = load_png_ex ("images/blocks/blade.png");

  tileSprites[tile_wall][0] = load_png_ex ("images/blocks/wall.png");
  tileSprites[tile_wall_slide][0] = load_png_ex ("images/blocks/wall_slide.png");


  tileSprites[tile_fire][0] = load_png_ex ("images/blocks/fire_0.png");
  tileSprites[tile_fire][1] = load_png_ex ("images/blocks/fire_1.png");
  tileSprites[tile_fire][2] = load_png_ex ("images/blocks/fire_2.png");
  tileSprites[tile_fire][3] = load_png_ex ("images/blocks/fire_3.png");

  tileSprites[tile_water][0] = load_png_ex ("images/blocks/water_1.png");
  tileSprites[tile_water][1] = load_png_ex ("images/blocks/water_2.png");
  tileSprites[tile_water][2] = load_png_ex ("images/blocks/water_3.png");
  tileSprites[tile_water][3] = load_png_ex ("images/blocks/water_4.png");


  tileSprites[tile_spawn_player][0] = load_png_ex ("images/blocks/spawn1.png");
  tileSprites[tile_spawn_player][1] = load_png_ex ("images/blocks/spawn2.png");
  tileSprites[tile_spawn_player][2] = load_png_ex ("images/blocks/spawn3.png");
  tileSprites[tile_spawn_player][3] = load_png_ex ("images/blocks/spawn4.png");

  // Toaster
  tileSprites[tile_toaster_block][0] = load_png_ex ("images/blocks/toaster_block.png");
  tileSprites[tile_toaster_left][0] = load_png_ex ("images/blocks/toaster_left.png");
  tileSprites[tile_toaster_right][0] = load_png_ex ("images/blocks/toaster_right.png");
  tileSprites[tile_toaster_center][0] = load_png_ex ("images/blocks/toaster_center.png");

  tileSprites[tile_toaster_element][0] = load_png_ex ("images/blocks/heatPanel1.png");
  tileSprites[tile_toaster_element][1] = load_png_ex ("images/blocks/heatPanel2.png");
  tileSprites[tile_toaster_element][2] = load_png_ex ("images/blocks/heatPanel3.png");
  tileSprites[tile_toaster_element][3] = load_png_ex ("images/blocks/heatPanel4.png");


  tileSprites[tile_claw_extension][0] = load_png_ex ("images/blocks/pawExtension1.png");
  tileSprites[tile_claw_extension][1] = load_png_ex ("images/blocks/pawExtension2.png");
  tileSprites[tile_claw_extension][2] = load_png_ex ("images/blocks/pawExtension3.png");
  tileSprites[tile_claw_extension][3] = load_png_ex ("images/blocks/pawExtension4.png");

  tileSprites[tile_bed1][0] = load_png_ex ("images/blocks/bed1.png");
  tileSprites[tile_bed2][0] = load_png_ex ("images/blocks/bed2.png");
  tileSprites[tile_bed3][0] = load_png_ex ("images/blocks/bed3.png");
  tileSprites[tile_pillow][0] = load_png_ex ("images/blocks/pillow.png");

  tileSprites[tile_grass][0] = load_png_ex ("images/blocks/grass.png");
  tileSprites[tile_dirt][0] = load_png_ex ("images/blocks/dirt.png");
  tileSprites[tile_coca][0] = load_png_ex ("images/blocks/coca-ine.png");

  tileSprites[tile_element][0] = load_png_ex ("images/blocks/element0.png");
  tileSprites[tile_element][1] = load_png_ex ("images/blocks/element1.png");
  tileSprites[tile_element][2] = load_png_ex ("images/blocks/element2.png");
  tileSprites[tile_element][3] = load_png_ex ("images/blocks/element3.png");

  tileSprites[tile_car][0] = load_png_ex ("images/blocks/car.png");
  tileSprites[tile_red_cloth][0] = load_png_ex ("images/blocks/red_cloth.png");
  tileSprites[tile_wood_light][0] = load_png_ex ("images/blocks/wood_light.png");
  tileSprites[tile_wood_light_slide][0] = load_png_ex ("images/blocks/wood_light_slide.png");
  tileSprites[tile_ovenright][0] = load_png_ex ("images/blocks/ovenright.png");
  tileSprites[tile_ovenleft][0] = load_png_ex ("images/blocks/ovenleft.png");
  tileSprites[tile_oven_fill][0] = load_png_ex ("images/blocks/ovenfill.png");
  tileSprites[tile_oven_top][0] = load_png_ex ("images/blocks/oventop.png");

  tileSprites[tile_ovencenter][0] = load_png_ex ("images/blocks/ovenmiddle0.png");
  tileSprites[tile_ovencenter][1] = load_png_ex ("images/blocks/ovenmiddle1.png");
  tileSprites[tile_ovencenter][2] = load_png_ex ("images/blocks/ovenmiddle0.png");
  tileSprites[tile_ovencenter][3] = load_png_ex ("images/blocks/ovenmiddle1.png");

  tileSprites[tile_pipe1][0] = load_png_ex ("images/blocks/pipe_1.png");
  tileSprites[tile_pipe2][0] = load_png_ex ("images/blocks/pipe_2.png");
  tileSprites[tile_fork1][0] = load_png_ex ("images/blocks/fork.png");
  tileSprites[tile_fork2][0] = load_png_ex ("images/blocks/fork2.png");

  tileSprites[tile_light][0] = load_png_ex ("images/blocks/light.png");
  tileSprites[tile_light][1] = load_png_ex ("images/blocks/light.png");
  tileSprites[tile_light][2] = load_png_ex ("images/blocks/light.png");
  tileSprites[tile_light][3] = load_png_ex ("images/blocks/light.png");

  //Back Tiles
  tileSprites[tile_back_grey][0] = load_png_ex ("images/blocks/backs/grey.png");

  tileSprites[tile_back_grey_shadow_top][0] = load_png_ex ("images/blocks/backs/grey_shadow_top.png");
  tileSprites[tile_back_grey_shadow_top_right][0] = load_png_ex ("images/blocks/backs/grey_shadow_top_right.png");
  tileSprites[tile_back_grey_shadow_top_left][0] = load_png_ex ("images/blocks/backs/grey_shadow_top_left.png");
  tileSprites[tile_back_grey_shadow_left][0] = load_png_ex ("images/blocks/backs/grey_shadow_left.png");
  tileSprites[tile_back_grey_shadow_bottom_left][0] = load_png_ex ("images/blocks/backs/grey_shadow_bottom_left.png");
  tileSprites[tile_back_grey_shadow_top_left_corner][0] = load_png_ex ("images/blocks/backs/grey_shadow_top_left_corner.png");

  tileSprites[tile_back_grey_dark][0] = load_png_ex ("images/blocks/backs/grey_dark.png");
  tileSprites[tile_back_grey_dark_top_left][0] = load_png_ex ("images/blocks/backs/grey_dark_top_left.png");
  tileSprites[tile_back_grey_dark_top_right][0] = load_png_ex ("images/blocks/backs/grey_dark_top_right.png");

  tileSprites[tile_back_lamp][0] = load_png_ex ("images/blocks/backs/lamp.png");

  set_window_title ("Mini Jim");
}

void Init::update(StateEngine *engine) {
  setNextState (engine, StateEngine::STATE_INTRO);
}

void Init::draw(BITMAP *buffer) {
  clear_to_color(buffer, 0x000000);
}

Init::~Init() {

}
