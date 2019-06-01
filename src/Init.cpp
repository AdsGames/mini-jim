#include "Init.h"

#include <allegro.h>

#include "globals.h"
#include "utility/tools.h"

Init::Init() {
  set_window_title ("Setting up");

  // Setup number generator
  srand (time (NULL));

  // Tile Sprites
  // Not animated
  tileSprites[tile_air][0] = load_png ("images/blocks/air.png", NULL);
  tileSprites[tile_brick_red][0] = load_png ("images/blocks/brick_red.png", NULL);
  tileSprites[tile_finish][0] = load_png ("images/blocks/finish.png", NULL);

  tileSprites[tile_checkpoint][0] = load_png ("images/blocks/checkpoint0.png", NULL);
  tileSprites[tile_checkpoint][1] = load_png ("images/blocks/checkpoint1.png", NULL);
  tileSprites[tile_checkpoint][2] = load_png ("images/blocks/checkpoint2.png", NULL);
  tileSprites[tile_checkpoint][3] = load_png ("images/blocks/checkpoint3.png", NULL);

  // New tiles
  tileSprites[tile_carpet][0] = load_png ("images/blocks/carpet.png", NULL);
  tileSprites[tile_cement][0] = load_png ("images/blocks/cement.png", NULL);
  tileSprites[tile_cement_full][0] = load_png ("images/blocks/cement_full.png", NULL);
  tileSprites[tile_dice][0] = load_png ("images/blocks/dice.png", NULL);
  tileSprites[tile_tile][0] = load_png ("images/blocks/tile.png", NULL);
  tileSprites[tile_book][0] = load_png ("images/blocks/book.png", NULL);


  tileSprites[tile_carpet_top][0] = load_png ("images/blocks/carpet_top.png", NULL);
  tileSprites[tile_water_deep][0] = load_png ("images/blocks/water_deep.png", NULL);
  tileSprites[tile_bottle][0] = load_png ("images/blocks/bottle.png", NULL);
  tileSprites[tile_wood][0] = load_png ("images/blocks/wood.png", NULL);
  tileSprites[tile_stained_glass][0] = load_png ("images/blocks/glass.png", NULL);
  tileSprites[tile_white_tile][0] = load_png ("images/blocks/white_tile.png", NULL);
  tileSprites[tile_blue_wallpaper][0] = load_png ("images/blocks/blue_wallpaper.png", NULL);
  tileSprites[tile_blue_wallpaper_bottom][0] = load_png ("images/blocks/blue_wallpaper_bottom.png", NULL);
  tileSprites[tile_faucet][0] = load_png ("images/blocks/faucet.png", NULL);
  tileSprites[tile_tack][0] = load_png ("images/blocks/tack.png", NULL);
  tileSprites[tile_mousetrap_1][0] = load_png ("images/blocks/mousetrap_1.png", NULL);
  tileSprites[tile_mousetrap_2][0] = load_png ("images/blocks/mousetrap_2.png", NULL);

  tileSprites[tile_chicken][0] = load_png ("images/blocks/chicken_1.png", NULL);
  tileSprites[tile_chicken][1] = load_png ("images/blocks/chicken_4.png", NULL);
  tileSprites[tile_chicken][2] = load_png ("images/blocks/chicken_3.png", NULL);
  tileSprites[tile_chicken][3] = load_png ("images/blocks/chicken_2.png", NULL);
  tileSprites[tile_chicken][4] = load_png ("images/blocks/chicken_1.png", NULL);
  tileSprites[tile_chicken][5] = load_png ("images/blocks/chicken_1.png", NULL);
  tileSprites[tile_chicken][6] = load_png ("images/blocks/chicken_1.png", NULL);
  tileSprites[tile_chicken][7] = load_png ("images/blocks/chicken_1.png", NULL);


  tileSprites[tile_claw][0] = load_png ("images/blocks/claw1.png", NULL);
  tileSprites[tile_claw][1] = load_png ("images/blocks/claw2.png", NULL);
  tileSprites[tile_claw][2] = load_png ("images/blocks/claw3.png", NULL);
  tileSprites[tile_claw][3] = load_png ("images/blocks/claw4.png", NULL);

  tileSprites[tile_beak][0] = load_png ("images/blocks/beak.png", NULL);

  tileSprites[tile_glasses][0] = load_png ("images/blocks/glasses.png", NULL);
  tileSprites[tile_plate][0] = load_png ("images/blocks/plate.png", NULL);
  tileSprites[tile_plate_stack][0] = load_png ("images/blocks/plate_stack.png", NULL);
  tileSprites[tile_blade][0] = load_png ("images/blocks/blade.png", NULL);

  tileSprites[tile_wall][0] = load_png ("images/blocks/wall.png", NULL);
  tileSprites[tile_wall_slide][0] = load_png ("images/blocks/wall_slide.png", NULL);


  tileSprites[tile_fire][0] = load_png ("images/blocks/fire_0.png", NULL);
  tileSprites[tile_fire][1] = load_png ("images/blocks/fire_1.png", NULL);
  tileSprites[tile_fire][2] = load_png ("images/blocks/fire_2.png", NULL);
  tileSprites[tile_fire][3] = load_png ("images/blocks/fire_3.png", NULL);

  tileSprites[tile_water][0] = load_png ("images/blocks/water_1.png", NULL);
  tileSprites[tile_water][1] = load_png ("images/blocks/water_2.png", NULL);
  tileSprites[tile_water][2] = load_png ("images/blocks/water_3.png", NULL);
  tileSprites[tile_water][3] = load_png ("images/blocks/water_4.png", NULL);


  tileSprites[tile_spawn_player][0] = load_png ("images/blocks/spawn1.png", NULL);
  tileSprites[tile_spawn_player][1] = load_png ("images/blocks/spawn2.png", NULL);
  tileSprites[tile_spawn_player][2] = load_png ("images/blocks/spawn3.png", NULL);
  tileSprites[tile_spawn_player][3] = load_png ("images/blocks/spawn4.png", NULL);

  // Toaster
  tileSprites[tile_toaster_block][0] = load_png ("images/blocks/toaster_block.png", NULL);
  tileSprites[tile_toaster_left][0] = load_png ("images/blocks/toaster_left.png", NULL);
  tileSprites[tile_toaster_right][0] = load_png ("images/blocks/toaster_right.png", NULL);
  tileSprites[tile_toaster_center][0] = load_png ("images/blocks/toaster_center.png", NULL);

  tileSprites[tile_toaster_element][0] = load_png ("images/blocks/heatPanel1.png", NULL);
  tileSprites[tile_toaster_element][1] = load_png ("images/blocks/heatPanel2.png", NULL);
  tileSprites[tile_toaster_element][2] = load_png ("images/blocks/heatPanel3.png", NULL);
  tileSprites[tile_toaster_element][3] = load_png ("images/blocks/heatPanel4.png", NULL);


  tileSprites[tile_claw_extension][0] = load_png ("images/blocks/pawExtension1.png", NULL);
  tileSprites[tile_claw_extension][1] = load_png ("images/blocks/pawExtension2.png", NULL);
  tileSprites[tile_claw_extension][2] = load_png ("images/blocks/pawExtension3.png", NULL);
  tileSprites[tile_claw_extension][3] = load_png ("images/blocks/pawExtension4.png", NULL);

  tileSprites[tile_bed1][0] = load_png ("images/blocks/bed1.png", NULL);
  tileSprites[tile_bed2][0] = load_png ("images/blocks/bed2.png", NULL);
  tileSprites[tile_bed3][0] = load_png ("images/blocks/bed3.png", NULL);
  tileSprites[tile_pillow][0] = load_png ("images/blocks/pillow.png", NULL);

  tileSprites[tile_grass][0] = load_png ("images/blocks/grass.png", NULL);
  tileSprites[tile_dirt][0] = load_png ("images/blocks/dirt.png", NULL);
  tileSprites[tile_coca][0] = load_png ("images/blocks/coca-ine.png", NULL);

  tileSprites[tile_element][0] = load_png ("images/blocks/element0.png", NULL);
  tileSprites[tile_element][1] = load_png ("images/blocks/element1.png", NULL);
  tileSprites[tile_element][2] = load_png ("images/blocks/element2.png", NULL);
  tileSprites[tile_element][3] = load_png ("images/blocks/element3.png", NULL);

  tileSprites[tile_car][0] = load_png ("images/blocks/car.png", NULL);
  tileSprites[tile_red_cloth][0] = load_png ("images/blocks/red_cloth.png", NULL);
  tileSprites[tile_wood_light][0] = load_png ("images/blocks/wood_light.png", NULL);
  tileSprites[tile_wood_light_slide][0] = load_png ("images/blocks/wood_light_slide.png", NULL);
  tileSprites[tile_ovenright][0] = load_png ("images/blocks/ovenright.png", NULL);
  tileSprites[tile_ovenleft][0] = load_png ("images/blocks/ovenleft.png", NULL);
  tileSprites[tile_oven_fill][0] = load_png ("images/blocks/ovenfill.png", NULL);
  tileSprites[tile_oven_top][0] = load_png ("images/blocks/oventop.png", NULL);

  tileSprites[tile_ovencenter][0] = load_png ("images/blocks/ovenmiddle0.png", NULL);
  tileSprites[tile_ovencenter][1] = load_png ("images/blocks/ovenmiddle1.png", NULL);
  tileSprites[tile_ovencenter][2] = load_png ("images/blocks/ovenmiddle0.png", NULL);
  tileSprites[tile_ovencenter][3] = load_png ("images/blocks/ovenmiddle1.png", NULL);

  tileSprites[tile_pipe1][0] = load_png ("images/blocks/pipe_1.png", NULL);
  tileSprites[tile_pipe2][0] = load_png ("images/blocks/pipe_2.png", NULL);
  tileSprites[tile_fork1][0] = load_png ("images/blocks/fork.png", NULL);
  tileSprites[tile_fork2][0] = load_png ("images/blocks/fork2.png", NULL);

  tileSprites[tile_light][0] = load_png ("images/blocks/light.png", NULL);
  tileSprites[tile_light][1] = load_png ("images/blocks/light.png", NULL);
  tileSprites[tile_light][2] = load_png ("images/blocks/light.png", NULL);
  tileSprites[tile_light][3] = load_png ("images/blocks/light.png", NULL);

  //Back Tiles
  tileSprites[tile_back_grey][0] = load_png ("images/blocks/backs/grey.png", NULL);

  tileSprites[tile_back_grey_shadow_top][0] = load_png ("images/blocks/backs/grey_shadow_top.png", NULL);
  tileSprites[tile_back_grey_shadow_top_right][0] = load_png ("images/blocks/backs/grey_shadow_top_right.png", NULL);
  tileSprites[tile_back_grey_shadow_top_left][0] = load_png ("images/blocks/backs/grey_shadow_top_left.png", NULL);
  tileSprites[tile_back_grey_shadow_left][0] = load_png ("images/blocks/backs/grey_shadow_left.png", NULL);
  tileSprites[tile_back_grey_shadow_bottom_left][0] = load_png ("images/blocks/backs/grey_shadow_bottom_left.png", NULL);
  tileSprites[tile_back_grey_shadow_top_left_corner][0] = load_png ("images/blocks/backs/grey_shadow_top_left_corner.png", NULL);

  tileSprites[tile_back_grey_dark][0] = load_png ("images/blocks/backs/grey_dark.png", NULL);
  tileSprites[tile_back_grey_dark_top_left][0] = load_png ("images/blocks/backs/grey_dark_top_left.png", NULL);
  tileSprites[tile_back_grey_dark_top_right][0] = load_png ("images/blocks/backs/grey_dark_top_right.png", NULL);

  tileSprites[tile_back_lamp][0] = load_png ("images/blocks/backs/lamp.png", NULL);

  set_window_title ("Mini Jim");
}

void Init::update(StateEngine *engine) {
  setNextState (engine, StateEngine::STATE_INTRO);
}

void Init::draw(BITMAP *buffer) {

}

Init::~Init() {

}
