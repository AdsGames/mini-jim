#include "tile.h"

tile::tile(int newType){
  setType(newType);
}

//Getters/ setters
int tile::getX(){
  return x;
}
int tile::getY(){
  return y;
}

int tile::getWidth(){
  return width;
}
int tile::getHeight(){
  return height;
}

void tile::setX(int newX){
  x = newX;
  initialX = x;
}
void tile::setY(int newY){
  y = newY;
  initialY = y;
}
int tile::getType(){
  return type;
}
vector<int> tile::getAttribute(){
  return attribute;
}
// Contains Attribute
bool tile::containsAttribute( int newAttribute){
  if (find(attribute.begin(), attribute.end(), newAttribute) != attribute.end()) {
  	return true;
	}
	return false;
}

BITMAP* tile::getImage(){
  return images[0];
}

//Set type
void tile::setType(int newType){
  x = initialX;
  y = initialY;

  type = newType;
  attribute.clear();

  // Fronts
  if(type == tile_air){
    attribute.push_back(gas);
  }
  else if(type == tile_brick_red){
    attribute.push_back(solid);
  }
  else if(type == tile_carpet){
    attribute.push_back(solid);
  }
  else if(type == tile_carpet_top){
    attribute.push_back(solid);
  }
  else if(type == tile_tack){
    attribute.push_back(half_block_bottom);
    attribute.push_back(harmful);
  }
  else if(type == tile_cement){
    attribute.push_back(solid);
  }
  else if(type == tile_dice){
    attribute.push_back(solid);
  }
  else if(type == tile_tile){
    attribute.push_back(gas);
  }
  else if(type == tile_water){
    attribute.push_back(harmful);
  }
  else if(type == tile_book){
    attribute.push_back(solid);
  }
  else if(type == tile_toaster_block){
    attribute.push_back(solid);
  }
  else if(type == tile_toaster_left){
    attribute.push_back(solid);
  }
  else if(type == tile_toaster_right){
    attribute.push_back(solid);
  }
  else if(type == tile_toaster_center){
    attribute.push_back(solid);
  }
  else if(type == tile_toaster_element){
    attribute.push_back(harmful);
    attribute.push_back(light);
  }
  else if(type == tile_water_deep){
    attribute.push_back(harmful);
  }
  else if(type == tile_bottle){
    attribute.push_back(gas);
  }
  else if(type == tile_wood){
    attribute.push_back(solid);
  }
  else if(type == tile_stained_glass){
    attribute.push_back(gas);
  }
  else if(type == tile_white_tile){
    attribute.push_back(gas);
  }
  else if(type == tile_blue_wallpaper){
    attribute.push_back(gas);
  }
  else if(type == tile_blue_wallpaper_bottom){
    attribute.push_back(gas);
  }
  else if(type == tile_faucet){
    attribute.push_back(gas);
  }
  else if(type == tile_mousetrap_1){
    attribute.push_back(harmful);
  }
  else if(type == tile_mousetrap_2){
    attribute.push_back(solid);
  }
  else if(type == tile_beak){
    attribute.push_back(harmful);
  }
  else if(type == tile_plate_stack){
    attribute.push_back(solid);
  }
  else if(type == tile_plate){
    attribute.push_back(half_block_bottom);
    attribute.push_back(solid);
  }
  else if(type == tile_blade){
    attribute.push_back(quarter_block_bottom);
    attribute.push_back(harmful);
  }
  else if(type == tile_fire){
    attribute.push_back(half_block_bottom);
    attribute.push_back(harmful);
    attribute.push_back(light);
  }
  else if(type == tile_claw){
    attribute.push_back(harmful);
  }
  else if(type == tile_claw_extension){
    attribute.push_back(harmful);
  }
  else if(type == tile_bed1){
    attribute.push_back(solid);
  }
  else if(type == tile_bed2){
    attribute.push_back(solid);
  }
  else if(type == tile_bed3){
    attribute.push_back(solid);
  }
  else if(type == tile_pillow){
    attribute.push_back(solid);
  }
  else if(type == tile_coca){
    attribute.push_back(solid);
  }
  else if(type == tile_dirt){
    attribute.push_back(solid);
  }
  else if(type == tile_grass){
    attribute.push_back(solid);
  }
  else if(type == tile_element){
    attribute.push_back(harmful);
    attribute.push_back(light);
  }
  else if(type == tile_red_cloth){
    attribute.push_back(solid);
  }
  else if(type == tile_wood_light){
    attribute.push_back(solid);
  }
  else if(type == tile_ovenright){
    attribute.push_back(solid);
  }
  else if(type == tile_ovenleft){
    attribute.push_back(solid);
  }
  else if(type == tile_ovencenter){
    attribute.push_back(solid);
    attribute.push_back(light);
  }
  else if(type == tile_oven_fill){
    attribute.push_back(solid);
  }
  else if(type == tile_oven_top){
    attribute.push_back(solid);
  }
  else if(type == tile_pipe1){
    attribute.push_back(solid);
  }
  else if(type == tile_pipe2){
    attribute.push_back(solid);
  }
  else if(type == tile_fork1){
    attribute.push_back(solid);
    attribute.push_back(quarter_block_bottom);
  }
  else if(type == tile_fork2){
    attribute.push_back(harmful);
  }
   else if(type == tile_chicken){
    attribute.push_back(gas);
  }
   else if(type == tile_chicken_still){
    attribute.push_back(gas);
  }
  else if(type == tile_cement_full){
    attribute.push_back(solid);
  }
  else if(type == tile_wall){
    attribute.push_back(solid);
  }
  else if(type == tile_wall_slide){
    attribute.push_back(slide);
  }
  else if(type == tile_light){
    attribute.push_back(gas);
    attribute.push_back(light);
  }
  else if(type == tile_wood_light_slide){
    attribute.push_back(slide);
  }
  else{
    attribute.push_back(gas);
  }

  // Sets images
  if(tileSprites[type][4] != NULL){
    setImagesAnimatedEight(tileSprites[type][0], tileSprites[type][1], tileSprites[type][2], tileSprites[type][3], tileSprites[type][4], tileSprites[type][5], tileSprites[type][6], tileSprites[type][7]);
  }
  else if(tileSprites[type][1] != NULL){
    setImagesAnimated(tileSprites[type][0], tileSprites[type][1], tileSprites[type][2], tileSprites[type][3]);
  }
  else{
    setImages(tileSprites[type][0]);
  }
}

//Set tiles images
void tile::setImagesAnimatedEight(BITMAP* image1, BITMAP* image2, BITMAP* image3, BITMAP* image4, BITMAP* image5, BITMAP* image6, BITMAP* image7, BITMAP* image8){
  images[0] = image1;
  images[1] = image2;
  images[2] = image3;
  images[3] = image4;
  images[4] = image5;
  images[5] = image6;
  images[6] = image7;
  images[7] = image8;
  animated = 2;
  setDimensions();
}

//Set tiles images
void tile::setImagesAnimated(BITMAP* image1, BITMAP* image2, BITMAP* image3, BITMAP* image4){
  images[0] = image1;
  images[1] = image2;
  images[2] = image3;
  images[3] = image4;
  animated = 1;
  setDimensions();
}

//Set tiles images
void tile::setImages(BITMAP* image1){
  images[0] = image1;
  animated = 0;
  setDimensions();
}

// Set images dimensions
void tile::setDimensions(){
  if(images[0] != NULL){
    width = images[0] -> w;
    height = images[0] -> h;
    if( containsAttribute( half_block_top)){
      height = 32;
    }
    else if( containsAttribute( half_block_bottom)){
      height = 32;
      y += height;
    }
    else if( containsAttribute( quarter_block_top)){
      height = 16;
    }
    else if( containsAttribute( quarter_block_bottom)){
      height = 16;
      y += 48;
    }
  }
}

//Draw tile
void tile::draw_tile(BITMAP* tempSprite, int xOffset, int yOffset, int newFrame){
  if(images[0] != NULL){
    //Not animated
    if( animated == 0){
      draw_sprite(tempSprite, images[0], x - xOffset, y - yOffset);
    }
    //Animated?
    else if(animated == 1){
      if( newFrame > 3){
        newFrame = newFrame - 4;
      }
      if(images[newFrame] != NULL){
        draw_sprite(tempSprite, images[newFrame], x - xOffset, y - yOffset);
      }
      else{
        textprintf_ex(tempSprite,font,x - xOffset, y - yOffset,makecol(0,0,0),-1,"frame");
        textprintf_ex(tempSprite,font,x - xOffset, y - yOffset + 20,makecol(0,0,0),-1,"%i NA", newFrame + 1);
      }
    }
    else if(animated == 2){
      if(images[newFrame] != NULL){
        draw_sprite(tempSprite, images[newFrame], x - xOffset, y - yOffset);
      }
      else{
        textprintf_ex(tempSprite,font,x - xOffset, y - yOffset,makecol(0,0,0),-1,"frame");
        textprintf_ex(tempSprite,font,x - xOffset, y - yOffset + 20,makecol(0,0,0),-1,"%i NA", newFrame + 1);
      }
    }
  }
  //Image not available? draw debug
  else{
    textprintf_ex(tempSprite,font,x - xOffset, y - yOffset,makecol(0,0,0),-1,"Image");
    textprintf_ex(tempSprite,font,x - xOffset, y - yOffset + 20,makecol(0,0,0),-1,"%i NA", type);
  }
}

tile::~tile(){

}
