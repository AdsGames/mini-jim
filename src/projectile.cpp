#include "projectile.h"

projectile::projectile(int newType, int newX, int newY, int newSpeed){
  x = newX;
  y = newY;
  speed = newSpeed;
  type = 0;
  contact_counter = 0;
  contact = false;
  setType(newType);
  setSounds(newType);
  play_sample(sound[0],255,125,1000,0);
}

projectile::~projectile(){

}

//Getters/ setters
int projectile::getX(){
  return x;
}
int projectile::getY(){
  return y;
}
int projectile::getType(){
  return type;
}
int projectile::getContactFrameCounter(){
  return contact_counter;
}

bool projectile::getContact(tileMap *newMap){
  //Check for collision
  for(int i = 0; i < newMap -> mapTiles.size(); i++){
    if(newMap -> mapTiles.at(i).containsAttribute(solid)){
      if(collisionAny(x, x + image[0] -> w, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() + 64, y, y + image[0] -> h, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() + 64)){
        contact = true;
      }
    }
  }

  if(!contact){
    if(x<0 || x > newMap -> width * 64 || y<0 || y > newMap -> height * 64){
      contact = true;
    }
  }
  else{
    if(contact_counter == 0){
      play_sample(sound[1],255,125,1000,0);
      speed = 0;
    }
    else if(contact_counter > 9){
      contact_counter = 10;
    }
    contact_counter++;
  }
  return contact;
}

//Set type
void projectile::setType(int newType){
  type = newType;

  //Sets images
  setImage(projectileSprites[type][0], projectileSprites[type][1], projectileSprites[type][2]);
}

//Set tiles images
void projectile::setImage(BITMAP* image1, BITMAP* image2, BITMAP* image3){
  image[0] = image1;
  image[1] = image2;
  image[2] = image3;
}

// Set Images
void projectile::setSounds( int newType){
  if(newType == 0){
    sound[0] = load_sample("sounds/shoot.wav");
    sound[1] = load_sample("sounds/shot_hit.wav");
  }
}

//Update
void projectile::update(){
  x += speed;
}

//Draw projectile
void projectile::draw(BITMAP* tempSprite, int xOffset, int yOffset){
  if(!contact){
    draw_sprite(tempSprite, image[0], x - xOffset, y - yOffset);
  }
  else{
    draw_sprite(tempSprite, image[2], x - xOffset, y - yOffset);
  }
}
