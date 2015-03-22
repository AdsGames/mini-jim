#include "player.h"

player::player(){

  x = 128;
  y = 128;

  deathcount;

  canFall = false;
  jumping = false;
  dead = false;
  sliding = false;
  finished = false;

  characterDir = UP;

  walking_animation_sequence = 0;

  jump_height = 0;
  yVelocity = 0;

  upKey = 0;
  downKey = 0;
  leftKey = 0;
  rightKey = 0;
  jumpKey = 0;
  sprintSpeed = 8;

  checkpointPosition[0] = 0;
  checkpointPosition[1] = 0;

  newMap = new tileMap("blank");
}

// 0-3 left, 4-7 right, 8-11 up
void player::load_images( int newType){
  if( newType == 1){
    player_images[0] = load_bitmap ( "images/character/character_1_left_1.png", NULL);
    player_images[1] = load_bitmap ( "images/character/character_1_left_2.png", NULL);
    player_images[2] = load_bitmap ( "images/character/character_1_left_3.png", NULL);
    player_images[3] = load_bitmap ( "images/character/character_1_left_4.png", NULL);

    player_images[4] = load_bitmap ( "images/character/character_1_right_1.png", NULL);
    player_images[5] = load_bitmap ( "images/character/character_1_right_2.png", NULL);
    player_images[6] = load_bitmap ( "images/character/character_1_right_3.png", NULL);
    player_images[7] = load_bitmap ( "images/character/character_1_right_4.png", NULL);

    player_images[8] = load_bitmap ( "images/character/character_1_right.png", NULL);
    player_images[9] = load_bitmap ( "images/character/character_1_right.png", NULL);
    player_images[10] = load_bitmap ( "images/character/character_1_right.png", NULL);
    player_images[11] = load_bitmap ( "images/character/character_1_right.png", NULL);

    player_images[12] = load_bitmap ( "images/character/character_1_left_jump_1.png", NULL);
    player_images[13] = load_bitmap ( "images/character/character_1_left_jump_2.png", NULL);
    player_images[14] = load_bitmap ( "images/character/character_1_left_jump_2.png", NULL);
    player_images[15] = load_bitmap ( "images/character/character_1_left_jump_2.png", NULL);
    player_images[16] = load_bitmap ( "images/character/character_1_left_jump_2.png", NULL);
    player_images[17] = load_bitmap ( "images/character/character_1_left_jump_2.png", NULL);

    player_images[18] = load_bitmap ( "images/character/character_1_right_jump_1.png", NULL);
    player_images[19] = load_bitmap ( "images/character/character_1_right_jump_2.png", NULL);
    player_images[20] = load_bitmap ( "images/character/character_1_right_jump_2.png", NULL);
    player_images[21] = load_bitmap ( "images/character/character_1_right_jump_2.png", NULL);
    player_images[22] = load_bitmap ( "images/character/character_1_right_jump_2.png", NULL);
    player_images[23] = load_bitmap ( "images/character/character_1_right_jump_2.png", NULL);

    player_images[24] = load_bitmap ( "images/character/character_1_slide_left.png", NULL);
    player_images[25] = load_bitmap ( "images/character/character_1_slide_left.png", NULL);
    player_images[26] = load_bitmap ( "images/character/character_1_slide_right.png", NULL);
    player_images[27] = load_bitmap ( "images/character/character_1_slide_right.png", NULL);

    player_images[28] = load_bitmap ( "images/character/character_1_left_idle.png", NULL);
    player_images[29] = load_bitmap ( "images/character/character_1_right_idle.png", NULL);
  }
  else if( newType == 2){
    player_images[0] = load_bitmap ( "images/character/character_2_left_1.png", NULL);
    player_images[1] = load_bitmap ( "images/character/character_2_left_2.png", NULL);
    player_images[2] = load_bitmap ( "images/character/character_2_left_3.png", NULL);
    player_images[3] = load_bitmap ( "images/character/character_2_left_4.png", NULL);

    player_images[4] = load_bitmap ( "images/character/character_2_right_1.png", NULL);
    player_images[5] = load_bitmap ( "images/character/character_2_right_2.png", NULL);
    player_images[6] = load_bitmap ( "images/character/character_2_right_3.png", NULL);
    player_images[7] = load_bitmap ( "images/character/character_2_right_4.png", NULL);

    player_images[8] = load_bitmap ( "images/character/character_2_right.png", NULL);
    player_images[9] = load_bitmap ( "images/character/character_2_right.png", NULL);
    player_images[10] = load_bitmap ( "images/character/character_2_right.png", NULL);
    player_images[11] = load_bitmap ( "images/character/character_2_right.png", NULL);

    player_images[12] = load_bitmap ( "images/character/character_2_left_jump_1.png", NULL);
    player_images[13] = load_bitmap ( "images/character/character_2_left_jump_2.png", NULL);
    player_images[14] = load_bitmap ( "images/character/character_2_left_jump_2.png", NULL);
    player_images[15] = load_bitmap ( "images/character/character_2_left_jump_2.png", NULL);
    player_images[16] = load_bitmap ( "images/character/character_2_left_jump_2.png", NULL);
    player_images[17] = load_bitmap ( "images/character/character_2_left_jump_2.png", NULL);

    player_images[18] = load_bitmap ( "images/character/character_2_right_jump_1.png", NULL);
    player_images[19] = load_bitmap ( "images/character/character_2_right_jump_2.png", NULL);
    player_images[20] = load_bitmap ( "images/character/character_2_right_jump_2.png", NULL);
    player_images[21] = load_bitmap ( "images/character/character_2_right_jump_2.png", NULL);
    player_images[22] = load_bitmap ( "images/character/character_2_right_jump_2.png", NULL);
    player_images[23] = load_bitmap ( "images/character/character_2_right_jump_2.png", NULL);

    player_images[24] = load_bitmap ( "images/character/character_2_slide_left.png", NULL);
    player_images[25] = load_bitmap ( "images/character/character_2_slide_left.png", NULL);
    player_images[26] = load_bitmap ( "images/character/character_2_slide_right.png", NULL);
    player_images[27] = load_bitmap ( "images/character/character_2_slide_right.png", NULL);

    player_images[28] = load_bitmap ( "images/character/character_2_left_idle.png", NULL);
    player_images[29] = load_bitmap ( "images/character/character_2_right_idle.png", NULL);
  }

  projectileSprites[0][0] = load_bitmap("images/laser.png", NULL);
  projectileSprites[0][1] = load_bitmap("images/laser.png", NULL);
  projectileSprites[0][2] = load_bitmap("images/laser_hit.png", NULL);
}

// Load sounds
void player::load_sounds(){
  if(!(chicken = load_sample("sounds/chicken.wav"))){
    abort_on_error( "Cannot find sound sounds/chicken.wav \n Please check your files and try again");
  }
  if(!(walk1 = load_sample("sounds/walk_1.wav"))){
    abort_on_error( "Cannot find sound sounds/walk_1.wav \n Please check your files and try again");
  }
  if(!(walk2 = load_sample("sounds/walk_2.wav"))){
    abort_on_error( "Cannot find sound sounds/walk_2.wav \n Please check your files and try again");
  }
  if(!(jump = load_sample("sounds/jump.wav"))){
    abort_on_error( "Cannot find sound sounds/jump.wav \n Please check your files and try again");
  }
  if(!(die = load_sample("sounds/die.wav"))){
    abort_on_error( "Cannot find sound sounds/die.wav \n Please check your files and try again");
  }
  if(!(getItem = load_sample("sounds/get_item.wav"))){
    abort_on_error( "Cannot find sound sounds/getItem.wav \n Please check your files and try again");
  }
  if(!(getBonus = load_sample("sounds/get_bonus.wav"))){
    abort_on_error( "Cannot find sound sounds/getBonus.wav \n Please check your files and try again");
  }
  if(!(win = load_sample("sounds/win.wav"))){
    abort_on_error( "Cannot find sound sounds/win.wav \n Please check your files and try again");
  }
  if(!(trapsnap = load_sample("sounds/trapsnap.wav"))){
    abort_on_error( "Cannot find sound sounds/trapsnap.wav \n Please check your files and try again");
  }
  if(!(checkpoint = load_sample("sounds/checkpoint.wav"))){
    abort_on_error( "Cannot find sound sounds/checkpoint.wav \n Please check your files and try again");
  }
}

// Set keys
void player::set_keys( int up, int down, int left, int right, int jump, int newJoyNumber){
  // Differentiate controls
  upKey = up;
  downKey = down;
  leftKey = left;
  rightKey = right;
  jumpKey = jump;
  joyNumber = newJoyNumber;
}

int player::getDeathcount(){
  return deathcount;
}

// Return X
int player::getX(){
  return x;
}

// Return Y
int player::getY(){
  return y;
}

// Get finished
bool player::getFinished(){
  return finished;
}

// Dead?
bool player::getDead(){
  if(dead){
    x = 128;
    y = 128;
  }
  return dead;
}

//Get bullets
vector<projectile> player::getBullets(){
  return bullets;
}

// Set finished
void player::setFinished( bool newFinished){
    finished = newFinished;
}
//Set deathcount
void player::setDeathcount(int newDeathcount){
  deathcount=newDeathcount;
}
//Set dead
void player::setDead(bool newDead){
  dead = newDead;
}

//Draw character
void player::draw(BITMAP* temp, int tile_map_x, int tile_map_y){
  if( !sliding){
    if(jumping || canFall){
      if(characterDir == LEFT){
        draw_sprite( temp, player_images[17], x - tile_map_x, y - tile_map_y);
      }
      else{
        draw_sprite( temp, player_images[23], x - tile_map_x, y - tile_map_y);
      }
    }
    else if(characterDir + walking_animation_sequence/ANIMATION_SPEED==0 || characterDir + walking_animation_sequence/ANIMATION_SPEED==4){
      if(idle_timer<=50 && characterDir==LEFT)draw_sprite( temp, player_images[0], x - tile_map_x, y - tile_map_y);
      if(idle_timer<=50 && characterDir==RIGHT)draw_sprite( temp, player_images[4], x - tile_map_x, y - tile_map_y);
      if(idle_timer>50 && characterDir==LEFT)draw_sprite( temp, player_images[28], x - tile_map_x, y - tile_map_y);
      if(idle_timer>50 && characterDir==RIGHT)draw_sprite( temp, player_images[29], x - tile_map_x, y - tile_map_y);
    }else{
        draw_sprite( temp, player_images[characterDir + walking_animation_sequence/ANIMATION_SPEED], x - tile_map_x, y - tile_map_y);
    }
  }
  else{
    if(characterDir == LEFT){
      draw_sprite( temp, player_images[24], x - tile_map_x, y - tile_map_y);
    }
    else{
      draw_sprite( temp, player_images[26], x - tile_map_x, y - tile_map_y);
    }
  }
}

// Spawn
void player::spawncommand(tileMap *fullMap){
  if( checkpointPosition[0] != 0 && checkpointPosition[1] != 0){
    x = checkpointPosition[0];
    y = checkpointPosition[1];
  }
  else{
    for(int i = 0; i < fullMap -> mapTiles.size(); i++){
      if(fullMap -> mapTiles.at(i).getType() == 199){
        x = fullMap -> mapTiles.at(i).getX();
        y = fullMap -> mapTiles.at(i).getY();
      }
    }
  }
}

//Movement
void player::update(tileMap *fullMap){
  //Collision stuff
  bool canMoveLeft = true;
  bool canMoveRight = true;
  bool canClimbUp = true;
  bool canClimbDown = true;
  bool canClimbUp2 = false;
  bool canClimbDown2 = false;
  bool canJump = true;
  bool canJumpUp = true;
  bool inLiquid = false;


  newMap -> mapTiles.clear();
  newMap -> width = fullMap -> width;
  newMap -> height = fullMap -> height;

  // Add close elements
  for(int i = 0; i < fullMap -> mapTiles.size(); i++){
    if(collisionAny(x - 140, x + 140, fullMap -> mapTiles.at(i).getX(), fullMap -> mapTiles.at(i).getX() +  fullMap -> mapTiles.at(i).getWidth(), y - 140, y + 140, fullMap -> mapTiles.at(i).getY(), fullMap -> mapTiles.at(i).getY() + fullMap -> mapTiles.at(i).getHeight())){
        newMap -> mapTiles.push_back( fullMap -> mapTiles.at(i));

    }
  }

  //Check for collision
  for(int i = 0; i < newMap -> mapTiles.size(); i++){
    // Check moving LEFT
    if((newMap -> mapTiles.at(i).containsAttribute(solid) || newMap -> mapTiles.at(i).containsAttribute(slide))&& !sliding){
      if(collisionAny(x + 8 - sprintSpeed, x + 56, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() +  newMap -> mapTiles.at(i).getWidth(), y, y + 64, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight()) &&
         collisionLeft(x + 8 - sprintSpeed, x + 56, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() +  newMap -> mapTiles.at(i).getWidth())){
        canMoveLeft = false;
      }
    }
    //Sliding left
    if(newMap -> mapTiles.at(i).containsAttribute(solid)&& sliding){
      if(collisionAny(x + 8 - sprintSpeed, x + 56, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() +  newMap -> mapTiles.at(i).getWidth(), y, y + 64, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight()) &&
         collisionLeft(x + 8 - sprintSpeed, x + 56, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() +  newMap -> mapTiles.at(i).getWidth())){
        canMoveLeft = false;
      }
    }



    // Check moving RIGHT
    if((newMap -> mapTiles.at(i).containsAttribute(solid)|| newMap -> mapTiles.at(i).containsAttribute(slide))&& !sliding){
      if(collisionAny(x + 8, x + 56 + sprintSpeed, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() +  newMap -> mapTiles.at(i).getWidth(), y, y + 64, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight()) &&
         collisionRight(x + 8, x + 56 + sprintSpeed, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() +  newMap -> mapTiles.at(i).getWidth())){
        canMoveRight = false;
      }
    }
     if(newMap -> mapTiles.at(i).containsAttribute(solid)&& sliding){
      if(collisionAny(x + 8, x + 56 + sprintSpeed, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() +  newMap -> mapTiles.at(i).getWidth(), y, y + 64, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight()) &&
         collisionRight(x + 8, x + 56 + sprintSpeed, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() +  newMap -> mapTiles.at(i).getWidth())){
        canMoveRight = false;
      }
    }
    // Check 2 for climbing up
    if(newMap -> mapTiles.at(i).containsAttribute(climb)){
      if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() +  newMap -> mapTiles.at(i).getWidth(), y, y + 64, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight())){
        canClimbUp2 = true;
      }
    }
    // Check 2 for climbing down
    if(newMap -> mapTiles.at(i).containsAttribute(climb)){
      if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() +  newMap -> mapTiles.at(i).getWidth(), y, y + 72, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight())){
        canClimbDown2 = true;
      }
    }
    // Check 1 for climbing up
    if(newMap -> mapTiles.at(i).containsAttribute(solid)){
      if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() +  newMap -> mapTiles.at(i).getWidth(), y - 16, y, newMap -> mapTiles.at(i).getY(),newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight())){
        canClimbUp = false;
      }
    }
    // Check 2 for climbing down
    if(newMap -> mapTiles.at(i).containsAttribute(solid)){
      if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() +  newMap -> mapTiles.at(i).getWidth(), y, y + 72, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight())){
        canClimbDown = false;
      }
    }
    // Check jump
    if(!(newMap -> mapTiles.at(i).containsAttribute(gas)) && newMap -> mapTiles.at(i).containsAttribute(liquid)){
      if(collisionAny(x + 18, x + 44, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() +  newMap -> mapTiles.at(i).getWidth(), y, y + 64, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight())){
        canJump = false;
      }
    }
    // Can if you will not hit your head
    if(!(newMap -> mapTiles.at(i).containsAttribute(gas)) || newMap -> mapTiles.at(i).containsAttribute(liquid)){
      if(collisionAny(x + 18, x + 44, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() +  newMap -> mapTiles.at(i).getWidth(), y, y + 64, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight() + 1) &&
      collisionTop(newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight(), y - yVelocity, y + 64 - yVelocity)){
        canJumpUp = false;
      }
    }

    // If you are swimming
    if(newMap -> mapTiles.at(i).containsAttribute(liquid)){
      if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() +  newMap -> mapTiles.at(i).getWidth(), y-16, y + 64, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight())){
        inLiquid = true;
      }
    }
  }

  //Check for points and dangers
  for(int i = 0; i < fullMap -> mapTiles.size(); i++){

    if(fullMap -> mapTiles.at(i).containsAttribute(harmful)){
      if(collisionAny(x + 16, x + 48, fullMap -> mapTiles.at(i).getX(), fullMap -> mapTiles.at(i).getX() +  fullMap -> mapTiles.at(i).getWidth(), y - 16, y + 64, fullMap -> mapTiles.at(i).getY(), fullMap -> mapTiles.at(i).getY() +  fullMap -> mapTiles.at(i).getHeight())){
        if(fullMap -> mapTiles.at(i).getType() == tile_mousetrap_1){
          fullMap -> mapTiles.at(i).setType(tile_mousetrap_2);
          play_sample(trapsnap,255,125,1000,0);

        }
        else if(fullMap -> mapTiles.at(i).getType() == tile_beak){
          play_sample( chicken, 255, 128, 1000, 0);
        }
        else{
          play_sample(die,255,125,1000,0);
        }
        dead = true;
        deathcount++;
      }
    }
    // Chicken stuff
    if(fullMap -> mapTiles.at(i).getType() == tile_chicken){
      if(fullMap -> getFrame() == 1){
        fullMap -> mapTiles.at((fullMap -> width) * 8 + i + 1).setType( tile_beak);
      }
      else{
        fullMap -> mapTiles.at((fullMap -> width) * 8 + i + 1).setType( 0);
      }
    }
  }

  for(int i = 0; i < newMap -> mapTiles.size(); i++){
    // Checkpoint
    if(newMap -> mapTiles.at(i).getType() == tile_checkpoint){
      if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() +  newMap -> mapTiles.at(i).getWidth(), y - 16, y + 64, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight())){
        if( checkpointPosition[0] != newMap -> mapTiles.at(i).getX() || checkpointPosition[1] != newMap -> mapTiles.at(i).getY()){
          checkpointPosition[0] = newMap -> mapTiles.at(i).getX();
          checkpointPosition[1] = newMap -> mapTiles.at(i).getY();
          play_sample( checkpoint, 255, 128, 1000, 0);
        }
      }
    }

    //Die

    if(newMap -> mapTiles.at(i).getType() == tile_finish){
      if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() +  newMap -> mapTiles.at(i).getWidth(), y - 16, y + 64, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight())){
        play_sample(win,255,125,1000,0);
        finished = true;
      }
    }
    if(x > newMap -> width*64 || x < 0 || y > newMap -> height*64){
      x=0;
      y=0;
      play_sample(die,255,125,1000,0);
      dead = true;
      deathcount++;
    }
  }

  // Slide
  if((key[downKey] || joy[joyNumber].stick[0].axis[2].d1)){
    sliding = true;
  }
  else{
    if( sprintSpeed < 1){
      sprintSpeed = 8;
    }
    sliding = false;
  }

  //Move right
  if((key[rightKey] || joy[joyNumber].stick[0].axis[0].d2) && x < newMap -> width*64){
    if( !sprinting && !sliding){
      sprinting = true;
      sprintSpeed = 8;
    }
    else{
      if( sliding){
        if( sprintSpeed < 1){
          sprintSpeed = 0;
        }
        else{
          sprintSpeed /= 1.05;
        }
      }
      else if( sprintSpeed < 16 && !jumping){
        sprintSpeed *= 1.05;
      }
    }
    characterDir = RIGHT;
    if(canMoveRight){
      x += sprintSpeed;
      walking_animation_sequence++;
      if(walking_animation_sequence == 1*ANIMATION_SPEED && !canFall && !jumping){
        if(random(0,1)){
          play_sample(walk1,255,125,1000,0);
        }
        else{
          play_sample(walk2,255,125,1000,0);
        }
      }
    }
  }

  //Move
  if((key[leftKey] || joy[joyNumber].stick[0].axis[0].d1) && x > 0){
    if( !sprinting && !sliding){
      sprinting = true;
      sprintSpeed = 8;
    }
    else{
      if( sliding){
        if( sprintSpeed < 1){
          sprintSpeed = 0;
        }
        else{
          sprintSpeed /= 1.05;
        }
      }
      else if( sprintSpeed < 16 && !jumping){
        sprintSpeed *= 1.05;
      }
    }
    characterDir = LEFT;
    if(canMoveLeft){
      x -= sprintSpeed;
      walking_animation_sequence++;
      if(walking_animation_sequence == 1*ANIMATION_SPEED && !canFall && !jumping){
        if(random(0,1)){
          play_sample(walk1,255,125,1000,0);
        }
        else{
          play_sample(walk2,255,125,1000,0);
        }
      }
    }
  }

  // Stop sprinting
  if(!key[leftKey] && !key[rightKey] && !joy[joyNumber].stick[0].axis[0].d1 && !joy[joyNumber].stick[0].axis[0].d2){
    sprinting = false;
    sprintSpeed = 0;
  }

  canFall = true;
  bool smoothFall = false;
  int instalFallDistance = 0;

  //Falling (calculated separately to ensure collision accurate)
  for(int i = 0; i < newMap -> mapTiles.size(); i++){
    if(newMap -> mapTiles.at(i).containsAttribute(solid) || newMap -> mapTiles.at(i).containsAttribute(climb)){
      if(collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() +  newMap -> mapTiles.at(i).getWidth(), y, y + 96, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight()) &&
         collisionTop(y, y + 96, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight())){
        canFall = false;
        if(!collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() +  newMap -> mapTiles.at(i).getWidth(), y, y + 65, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight()) &&
         !collisionTop(y, y + 65, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight())){
          smoothFall = true;
          instalFallDistance = 1;
          if(!collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() +  newMap -> mapTiles.at(i).getWidth(), y, y + 69, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight()) &&
           !collisionTop(y, y + 69, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight())){
            instalFallDistance = 4;
            if(!collisionAny(x + 16, x + 48, newMap -> mapTiles.at(i).getX(), newMap -> mapTiles.at(i).getX() +  newMap -> mapTiles.at(i).getWidth(), y, y + 73, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight()) &&
             !collisionTop(y, y + 73, newMap -> mapTiles.at(i).getY(), newMap -> mapTiles.at(i).getY() +  newMap -> mapTiles.at(i).getHeight())){
              instalFallDistance = 8;
            }
          }
        }
      }
    }
  }

  //Fall
  if(canFall && !jumping){
    if(inLiquid){
      y += 16;
    }
    else{
      if( key[downKey]){
        y += 32;
      }
      else{
        y += 16;
      }
    }
  }
  //Smooth falling
  if(smoothFall && !jumping){
    y += instalFallDistance;
  }

  //Jumping
  if(jumping && canJumpUp){
    if( jump_height > 0 ){
      y -= yVelocity;
    }
    if( jump_height <= 0){
      if(yVelocity > -16){
        int newVelValue = 0;

        if( key[downKey] || joy[joyNumber].stick[0].axis[1].d2){
          newVelValue = 2;
        }
        else if( key[upKey]){
          newVelValue = 8;
        }
        else{
          newVelValue = 4;
        }

        if( yVelocity >= 1){
          yVelocity /= newVelValue;
          if( yVelocity < 1){
            yVelocity = -1;
          }
        }
        else{
          yVelocity *= newVelValue;
        }
      }
      else{
        yVelocity = 0;
        jumping = false;
      }
    }
    else{
      jump_height -= yVelocity;
    }
  }
  else if(jumping && !canJumpUp){
    jumping = false;
  }

  //Jump
  if((key[jumpKey] || key[upKey] || joy[joyNumber].button[0].b) && !sliding){
    if(inLiquid){
      y -= 16;
    }
    else if(!canFall && canJump && !jumping){
      yVelocity = 16;
      if( canJumpUp){
        play_sample(jump,255,125,1000,0);
        jump_height = 0;
        jumping = true;
        jump_height = 140;
      }
      else{
        yVelocity = 0;
      }
    }
  }

  // Reset animation sequence
  if(walking_animation_sequence >= ANIMATION_SPEED * 4){
    walking_animation_sequence = 0;
  }

  // Prevents getting stuck through an animation
  else if(!key[leftKey] && !key[rightKey] && !key[upKey] && !key[downKey] && !joy[joyNumber].stick[0].axis[0].d1 && !joy[joyNumber].stick[0].axis[0].d2 && !joy[joyNumber].stick[0].axis[2].d1){
    walking_animation_sequence = 0;
  }
  idle_timer++;
  if(idle_timer>100)idle_timer=0;
}

player::~player(){
  bullets.clear();
  for(int i = 0; i < 26; i++){
		if(player_images[i]){
			destroy_bitmap(player_images[i]);
		}
	}

  destroy_sample( walk1);
  destroy_sample( walk2);
  destroy_sample( jump);
  destroy_sample( die);
  destroy_sample( getItem);
  destroy_sample( getBonus);
}
