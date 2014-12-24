#include "Game.h"

volatile int Game::timer1 = 00;

Game::Game()
{
  // Init fmod
  FSOUND_Init (44100, 32, 0);
  install_joystick(JOY_TYPE_AUTODETECT);

  // Creates a random number generator (based on time)
  srand (time(NULL));

  // Other Sprites
  buffer = create_bitmap( 1280, 960);
  screen1 = create_bitmap( 1280, 480);
  screen2 = create_bitmap( 1280, 480);

  lightingEnabled = false;

  // Player
  player1.load_images(1);
  player1.load_sounds();
  player1.set_keys( KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ENTER_PAD, 0);

  player2.load_images(2);
  player2.load_sounds();
  player2.set_keys( KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE, 1);

  // Build a color lookup table for lighting effects
  get_palette(pal);
  create_light_table(&light_table, pal, 0, 0, 0, NULL);


  // Temporary fonts
  FONT *f1, *f2, *f3, *f4, *f5;

  //Sets Font
  if(!(f1 = load_font(("fonts/cooper_black.pcx"), NULL, NULL))){
    abort_on_error( "Cannot find font fonts/arial_black.png \n Please check your files and try again");
  }

  f2 = extract_font_range(f1, ' ', 'A'-1);
  f3 = extract_font_range(f1, 'A', 'Z');
  f4 = extract_font_range(f1, 'Z'+1, 'z');

  //Merge fonts
  cooper = merge_fonts(f4, f5 = merge_fonts(f2, f3));

  //Destroy temporary fonts
  destroy_font(f1);
  destroy_font(f2);
  destroy_font(f3);
  destroy_font(f4);
  destroy_font(f5);

  // Load images
  if(!(countdownImage = load_bitmap( ("images/321go.png"), NULL))){
    abort_on_error( "Cannot find image images/gui/321go.png \n Please check your files and try again");
  }
  if(!(countdown = load_sample("sounds/countdown.wav"))){
    abort_on_error( "Cannot find sound sounds/countdown.wav \n Please check your files and try again");
  }
  if(!(timeout = load_sample("sounds/timeout.wav"))){
    abort_on_error( "Cannot find sound sounds/timeout.wav \n Please check your files and try again");
  }
  if(!(darkness = load_bitmap("images/darkness.png", NULL))){
    abort_on_error( "Cannot find image images/darkness.png \n Please check your files and try again");
  }
  if(!(darkness_old = load_bitmap( "images/darkness.png", NULL))){
    abort_on_error( "Cannot find image images/darkness.png \n Please check your files and try again");
  }
  if(!(spotlight = load_bitmap( "images/spotlight.png", NULL))){
    abort_on_error( "Cannot find image images/spotlight.png \n Please check your files and try again");
  }
  if(!(spaceImage[0] = load_bitmap( "images/space0.png", NULL))){
    abort_on_error( "Cannot find image images/space0.png \n Please check your files and try again");
  }
  if(!(spaceImage[1] = load_bitmap( "images/space1.png", NULL))){
    abort_on_error( "Cannot find image images/space1.png \n Please check your files and try again");
  }
  if(!(spaceImage[2] = load_bitmap( "images/abutton0.png", NULL))){
    abort_on_error( "Cannot find image images/abutton0.png \n Please check your files and try again");
  }
  if(!(spaceImage[3] = load_bitmap( "images/abutton1.png", NULL))){
    abort_on_error( "Cannot find image images/abutton1.png \n Please check your files and try again");
  }
  if(!(results = load_bitmap( "images/gui/winscreen.png", NULL))){
    abort_on_error( "Cannot find image images/gui/winscreen.png \n Please check your files and try again");
  }

  // Load music
  if(!(waitingMusic = FSOUND_Stream_Open("sounds/music/BasicJim.mp3",2, 0, 0))){
      abort_on_error( "Cannot find music sounds/music/BasicJim.mp3 \n Please check your files and try again");
  }
  if(!(mainMusic = FSOUND_Stream_Open("sounds/music/BasicJimFull.mp3",2, 0, 0))){
      abort_on_error( "Cannot find music sounds/music/BasicJimFull.mp3 \n Please check your files and try again");
  }

  // Init
  init();
}

void Game::gameTicker(){
  timer1++;
}
END_OF_FUNCTION(gameTicker)

void Game::init(){
  // Timer
  LOCK_VARIABLE(timer1);
  LOCK_FUNCTION(gameTicker);
  install_int_ex(gameTicker, BPS_TO_TIMER(100));

  // Variables
  player1.setDead(false);
  player2.setDead(false);

  // Create map
  tile_map = new tileMap("blank");
  if( levelOn == 0){
    tile_map -> load( "data/bedroom");
  }
  else if( levelOn == 1){
    tile_map -> load( "data/kitchen");
  }
  else if( levelOn == 2){
    tile_map -> load( "data/basement");
  }

  //Create duplicate map with only solids for updating enemies
  newTileMap = new tileMap("blank");

  for(int i = 0; i < tile_map -> mapTiles.size(); i++){
    if(!tile_map -> mapTiles.at(i).containsAttribute(gas)){
      newTileMap -> mapTiles.push_back(tile_map -> mapTiles.at(i));
    }
  }

  totalTime[0] = 0;
  totalTime[1] = 0;

  deathFrame = false;

  // Play music
  FSOUND_Stream_Play(0,waitingMusic);
  FSOUND_Stream_Play(1,mainMusic);

  FSOUND_SetVolume (0, 255);
  FSOUND_SetVolume (1, 0);

  player1.spawncommand( tile_map);
  player2.spawncommand( tile_map);

  // Draw player two screen initial
  tile_map -> y = player2.getY() - 200;
  tile_map -> x = player2.getX() - 50;
  tile_map -> draw_map(screen2);

  // Draw player one screen initial
  tile_map -> y = player1.getY() - 200;
  tile_map -> x = player1.getX() - 50;
  tile_map -> draw_map(screen1);

  // Player ones turn
  turnOne = true;
  spawning = false;
  controlReady = false;
}

void Game::update(){
  poll_joystick();

  if(!spawning){
    // Spawn time
    if( !controlReady){
      if((key[KEY_SPACE] || joy[!turnOne].button[0].b) && !player1.getFinished() && !player2.getFinished()){
        play_sample( countdown, 255, 125, 1000, 0);
        spawning = true;
        timer1 = 0;
      }
    }
    else{
      // Character movements (runs only every 2nd loop)
      if(frames_done % 2 == 0){
        // Update if its their turn
        if( turnOne){
          player1.update(tile_map);
        }
        else{
          player2.update(tile_map);
        }
      }
    }
  }

  // End turn
  if((player1.getDead() || player2.getDead())){
    FSOUND_SetVolume (0, 255);
    FSOUND_SetVolume (1, 0);
    totalTime[!turnOne] += 1000;

    if( turnOne && player2.getFinished() || !turnOne && player1.getFinished()){

    }
    else{
      turnOne = !turnOne;
    }

    if( player1.getDead()){
      player1.setDead( false);
      player1.spawncommand(tile_map);
    }
    else if( player2.getDead()){
      player2.setDead( false);
      player2.spawncommand(tile_map);
    }
    controlReady = false;
  }

  // Finish
  if( player1.getFinished() && !player2.getFinished() && turnOne){
    totalTime[0] += timer1;
    turnOne = false;
  }
  if( player2.getFinished() && !player1.getFinished() && !turnOne){
    totalTime[1] += timer1;
    turnOne = true;
  }

  // Change level when both are done
  if( player1.getFinished() && player2.getFinished()){
    if( key[KEY_ENTER]){
      player1.setFinished( false);
      player2.setFinished( false);
      set_next_state(STATE_MENU);
    }
    timer1 = 0;
  }

  // Spawn character
  if( spawning){
    // Mute full song, play waiting song
    FSOUND_SetVolume (0, 255-(timer1*0.85));
    FSOUND_SetVolume (1, (timer1*0.85));

    // Start round
    if( timer1 > 100){
      FSOUND_SetVolume (0, 0);
      FSOUND_SetVolume (1, 255);
      spawning = false;
      controlReady = true;
      timer1 = 0;
    }
  }
  // Time limit per turn
  else if( controlReady){
    if( timer1 > 1000 && !(player1.getFinished() || player2.getFinished())){
      play_sample( timeout, 255, 128, 1000, 0);
      FSOUND_SetVolume (0, 255);
      FSOUND_SetVolume (1, 0);
      controlReady = false;
      totalTime[!turnOne] += 1000;
      timer1 = 0;
      if( turnOne && player2.getFinished() || !turnOne && player1.getFinished()){

      }
      else{
        turnOne = !turnOne;
      }
    }
  }

  // Scroll Map
  if( turnOne){
    currentPlayer = &player1;
  }
  else{
    currentPlayer = &player2;
  }

  // Scroll map
  if(currentPlayer -> getY() - tile_map -> y < 200 && tile_map -> y > 0){
    tile_map -> y -= 8;
  }
  if(currentPlayer -> getY() - tile_map -> y > 275 && tile_map -> y < tile_map -> height * 64 -  480){
    tile_map -> y += 8;
  }
  if(currentPlayer -> getX() - tile_map -> x < 500 && tile_map -> x > 0){
    tile_map -> x -= 8;
  }
  if(currentPlayer -> getX() - tile_map -> x > 480 && tile_map -> x < tile_map -> width * 64 - 1280){
    tile_map -> x += 8;
  }

  // Quick move
  if( spawning){
    tile_map -> y = currentPlayer -> getY() - 200;
    tile_map -> x = currentPlayer -> getX() - 50;
  }

  // Back to menu
  if( key[KEY_M]){
    set_next_state( STATE_MENU);
  }
}

void Game::draw(){
  // Black background (just in case)
  rectfill( buffer, 0, 0, 1280, 960, makecol(0,0,0));

  // Draw tiles and characters
  if( turnOne){
    tile_map -> draw_map(screen1);
    player1.draw(screen1, tile_map -> x, tile_map -> y);
  }
  else{
    tile_map -> draw_map(screen2);
    player2.draw(screen2, tile_map -> x, tile_map -> y);
  }

  // Lighting
  if( lightingEnabled){
    set_alpha_blender();
    draw_sprite( darkness, darkness_old, 0, 0);

    for(int i = 0; i < tile_map -> mapTiles.size(); i++){
      if((tile_map -> mapTiles.at(i).getX() >= tile_map -> x - tile_map -> mapTiles.at(i).getWidth()) && (tile_map -> mapTiles.at(i).getX() < tile_map -> x + 1280) &&
         (tile_map -> mapTiles.at(i).getY() >= tile_map -> y - tile_map -> mapTiles.at(i).getHeight()) && (tile_map -> mapTiles.at(i).getY() < tile_map -> y + 960)){
        if(tile_map -> mapTiles.at(i).containsAttribute(light)){
          stretch_sprite(darkness, spotlight, tile_map -> mapTiles.at(i).getX() - tile_map -> x + 32 - (tile_map -> mapTiles.at(i).getWidth() * 3),
            tile_map -> mapTiles.at(i).getY() - tile_map -> y + 32 - (tile_map -> mapTiles.at(i).getHeight() * 3), tile_map -> mapTiles.at(i).getWidth() * 6,
            tile_map -> mapTiles.at(i).getHeight() * 6);
        }
      }
    }

    if( turnOne){
      draw_trans_sprite(screen2, darkness, 0, 0);
      draw_sprite(darkness, spotlight, player1.getX() - tile_map -> x + 32 - (spotlight->w/2), player1.getY() - tile_map -> y + 32 - (spotlight->h/2));
      draw_trans_sprite(screen1, darkness, 0, 0);
    }
    else{
      draw_trans_sprite(screen1, darkness, 0, 0);
      draw_sprite(darkness, spotlight, player2.getX() - tile_map -> x + 32 - (spotlight->w/2), player2.getY() - tile_map -> y + 32 - (spotlight->h/2));
      draw_trans_sprite(screen2, darkness, 0, 0);
    }
  }

  // Draw split screens
  stretch_sprite( buffer, screen1, 0, 0, 1280, 960/2);
  stretch_sprite( buffer, screen2, 0, 960/2, 1280, 960/2);

  rectfill( buffer, 0, 480 - 8,  1280, 480 + 8, makecol( 0,0,0));

  rectfill( buffer, 0, 0, 1280, 16, makecol( 0,0,0));
  rectfill( buffer, 0, 0, 16, 960, makecol( 0,0,0));
  rectfill( buffer, 1264, 0, 1280, 960, makecol( 0,0,0));
  rectfill( buffer, 0, 944, 1280, 960, makecol( 0,0,0));

  rectfill( buffer, 20, 20, 320, 140, makecol( 0,0,0));
  rectfill( buffer, 20, 500, 320, 620, makecol( 0,0,0));

  // Ready?
  if( !spawning && !controlReady){
    if( turnOne){
      masked_stretch_blit(countdownImage, buffer, 100, 0, 94, 26, 165, 160, 940, 260);
      if( num_joysticks > 0){
        draw_sprite( buffer, spaceImage[tile_map -> getFrame()/4 + 2], 640 - 32, 380);
      }
      else{
        draw_sprite( buffer, spaceImage[tile_map -> getFrame()/4], 640 - 128, 380);
      }
    }
    else{
      masked_stretch_blit(countdownImage, buffer, 100, 0, 94, 26, 165, 645, 940, 260);
      if( num_joysticks > 1){
        draw_sprite( buffer, spaceImage[tile_map -> getFrame()/4 + 2], 640 - 32, 860);
      }
      else{
        draw_sprite( buffer, spaceImage[tile_map -> getFrame()/4], 640 - 128, 860);
      }
    }
  }

  // Spawn timer
  if( spawning){
    // Scroll Map
    if( turnOne){
      currentPlayer = &player1;
    }
    else{
      currentPlayer = &player2;
    }
    if(currentPlayer -> getY() - tile_map -> y < 200 && tile_map -> y > 0){
      tile_map -> y -= 64;
    }
    if(currentPlayer -> getY() - tile_map -> y > 275 && tile_map -> y < tile_map -> height * 64 -  480){
      tile_map -> y += 64;
    }
    if(currentPlayer -> getX() - tile_map -> x < 500 && tile_map -> x > 0){
      tile_map -> x -= 64;
    }
    if(currentPlayer -> getX() - tile_map -> x > 480 && tile_map -> x < tile_map -> width * 64 - 1280){
      tile_map -> x += 64;
    }

    // Mute full song, play waiting song
    FSOUND_SetVolume (0, 255-(timer1*0.85));
    FSOUND_SetVolume (1, (timer1*0.85));

    // Set screen in
    int drawYCoordinate;
    if( turnOne){
      drawYCoordinate = 160;
    }
    else{
      drawYCoordinate = 645;
    }

    // Timer 3..2..1..GO!
    if( timer1 <= 33){
      masked_stretch_blit(countdownImage, buffer, 0, 0, 14, 18, 570, drawYCoordinate, 140, 180);
    }
    else if( timer1 <= 66){
      masked_stretch_blit(countdownImage, buffer, 19, 0, 14, 18, 570, drawYCoordinate, 140, 180);
    }
    else if( timer1 <= 80){
      masked_stretch_blit(countdownImage, buffer, 39, 0, 14, 18, 570, drawYCoordinate, 140, 180);
    }
    else if( timer1 <= 100){
      masked_stretch_blit(countdownImage, buffer, 57, 0, 40, 18, 440, drawYCoordinate, 400, 180);
    }
  }
  else if( controlReady){
    // Draw timer to screen
    string newTime = convertDoubleToString(timer1/10);
    newTime.insert((newTime.length() - 1), ".");
    textprintf_ex(buffer,cooper,40,!turnOne * 480 + 80 ,makecol(255,255,255),-1,((string)(("Timer: " + newTime))).c_str());
  }

  // Draw timer to screen
  string player1TotalTime = convertIntToString(totalTime[0]/10);
  if( turnOne && controlReady){
    player1TotalTime = convertIntToString(totalTime[0]/10 + timer1/10);
  }
  player1TotalTime.insert((player1TotalTime.length() - 1), ".");
  textprintf_ex(buffer,cooper,40,40,makecol(255,255,255),-1,((string)(("Total Time: " + player1TotalTime))).c_str());

  string player2TotalTime = convertIntToString(totalTime[1]/10);
  if( !turnOne && controlReady){
    player2TotalTime = convertIntToString(totalTime[1]/10 + timer1/10);
  }
  player2TotalTime.insert((player2TotalTime.length() - 1), ".");
  textprintf_ex(buffer,cooper,40,520,makecol(255,255,255),-1,((string)(("Total Time: " + player2TotalTime))).c_str());

  // Change level when both are done
  if( player1.getFinished() && player2.getFinished()){
    set_alpha_blender();
    draw_trans_sprite( buffer, results, 0, 0);
    textprintf_ex(buffer,cooper,560,410,makecol(255,255,255),-1,"%i", totalTime[0]/100);
    textprintf_ex(buffer,cooper,560,470,makecol(255,255,255),-1,"%i", totalTime[1]/100);
    if( totalTime[0] < totalTime[1]){
      textprintf_ex(buffer,cooper,433,520,makecol(255,255,255),-1,"%i", 1);
      textprintf_ex(buffer,cooper,610,520,makecol(255,255,255),-1,"%i", (totalTime[1] - totalTime[1])/100);
    }
    else if( totalTime[0] > totalTime[1]){
      textprintf_ex(buffer,cooper,433,520,makecol(255,255,255),-1,"%i", 2);
      textprintf_ex(buffer,cooper,610,520,makecol(255,255,255),-1,"%i", (totalTime[0] - totalTime[1])/100);
    }
  }

  // Draw buffer
  stretch_sprite( screen, buffer, 0, 0, SCREEN_W, SCREEN_H);
}

Game::~Game()
{
  // Destroy images
  destroy_bitmap( buffer);
  destroy_bitmap( screen1);
  destroy_bitmap( screen2);

  // Destroy sounds
  destroy_sample( countdown);

  // Fade out
  highcolor_fade_out(16);

  // Stop music
  FSOUND_Stream_Stop(waitingMusic);
  FSOUND_Stream_Stop(mainMusic);

  // Clean up fmod
  FSOUND_Close();
}