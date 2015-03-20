#include "Editor.h"

Editor::Editor()
{
  layer = 1;

  // Other Sprites
  buffer = create_bitmap( SCREEN_W, SCREEN_H);

  // Create map
  tile_map = new tileMap("data/templates/blank64x48");

  // Create example tile
  exampleTile = new tile(0);
  exampleTile -> setType(0);
  exampleTile -> setX(0);
  exampleTile -> setY(0);

  FONT *f1, *f2, *f3, *f4, *f5;

  //Sets Font
  f1 = load_font("fonts/arial_black.pcx", NULL, NULL);
  f2 = extract_font_range(f1, ' ', 'A'-1);
  f3 = extract_font_range(f1, 'A', 'Z');
  f4 = extract_font_range(f1, 'Z'+1, 'z');

  //Merge fonts
  font = merge_fonts(f4, f5 = merge_fonts(f2, f3));

  //Destroy temporary fonts
  destroy_font(f1);
  destroy_font(f2);
  destroy_font(f3);
  destroy_font(f4);
  destroy_font(f5);

  edittext = ".txt";
  iter = edittext.begin();

  opening = false;
  saving = false;
}

void Editor::save(){
  //Name input
  if(keypressed()){
    int  newkey   = readkey();
    char ASCII    = newkey & 0xff;
    char scancode = newkey >> 8;

    // a character key was pressed; add it to the string
    if(ASCII >= 32 && ASCII <= 126 && edittext.length() < 25 && scancode != KEY_SPACE){
      // add the new char
      iter = edittext.insert(iter, ASCII);
      // increment both the caret and the iterator
      iter++;
    }
    // some other, "special" key was pressed; handle it here
    else{
      if(scancode == KEY_DEL){
        if(iter != edittext.end()){
          iter = edittext.erase(iter);
        }
      }
      if(scancode == KEY_BACKSPACE){
        if(iter != edittext.begin()){
           iter--;
           iter = edittext.erase(iter);
        }
      }
      if(scancode == KEY_RIGHT){
        if(iter != edittext.end()){
          iter++;
        }
      }
      if(scancode == KEY_LEFT){
        if(iter != edittext.begin()){
          iter--;
        }
      }
      if(scancode == KEY_ENTER){
        //Save fronts
        int widthCounter;
        string finalFile = "data/" + edittext;
        ofstream saveRaw1;
        saveRaw1.open(finalFile.c_str());

        widthCounter = 0;
        for(int i = 0; i < tile_map -> mapTiles.size(); i++){
          widthCounter++;
          if(widthCounter == tile_map -> width){
            saveRaw1 << tile_map -> mapTiles.at(i).getType() << "\n";
            widthCounter = 0;
          }
          else{
            saveRaw1 << tile_map -> mapTiles.at(i).getType() << " ";
          }
        }
        saveRaw1.close();

        //Save backs
        finalFile = "data/" + edittext.substr(0, edittext.size()-4) + "_back.txt";
        ofstream saveRaw2;
        saveRaw2.open(finalFile.c_str());

        widthCounter = 0;
        for(int i = 0; i < tile_map -> mapTilesBack.size(); i++){
          widthCounter++;
          if(widthCounter == tile_map -> width){
            saveRaw2 << tile_map -> mapTilesBack.at(i).getType() << "\n";
            widthCounter = 0;
          }
          else{
            saveRaw2 << tile_map -> mapTilesBack.at(i).getType() << " ";
          }
        }
        saveRaw2.close();
        saving = false;
      }
    }
  }
}

void Editor::open(){
  tile_map -> mapTiles.clear();
  tile_map -> mapTilesBack.clear();

  //Name input
  if(keypressed()){
    int  newkey   = readkey();
    char ASCII    = newkey & 0xff;
    char scancode = newkey >> 8;

    // a character key was pressed; add it to the string
    if(ASCII >= 32 && ASCII <= 126 && edittext.length() < 25 && scancode != KEY_SPACE){
      // add the new char
      iter = edittext.insert(iter, ASCII);
      // increment both the caret and the iterator
      iter++;
    }
    // some other, "special" key was pressed; handle it here
    else{
      if(scancode == KEY_DEL){
        if(iter != edittext.end()){
          iter = edittext.erase(iter);
        }
      }
      if(scancode == KEY_BACKSPACE){
        if(iter != edittext.begin()){
           iter--;
           iter = edittext.erase(iter);
        }
      }
      if(scancode == KEY_RIGHT){
        if(iter != edittext.end()){
          iter++;
        }
      }
      if(scancode == KEY_LEFT){
        if(iter != edittext.begin()){
          iter--;
        }
      }
      if(scancode == KEY_ENTER){
        string finalFile = "data/" + edittext.substr(0, edittext.size()-4);
        tile_map -> load(finalFile);
        opening = false;
      }
    }
  }
}

void Editor::update(){
  // Back to menu
  if( key[KEY_M] && !saving && !opening){
    set_next_state( STATE_MENU);
  }

  // Close menu
  if(key[KEY_TILDE]){
    clear_keybuf();
    opening = false;
    saving = false;
  }

  if( saving){
    save();
  }
  else if( opening){
    open();
  }
  else{
    // Scroll Map
    if(mouse_y < 10 && tile_map -> y > 0){
       tile_map -> y -= 16;
    }
    if(mouse_y > (SCREEN_H-10) && tile_map -> y < tile_map -> height * 64 -  SCREEN_H){
       tile_map -> y += 16;
    }
    if(mouse_x  < 10 && tile_map -> x > 0){
       tile_map -> x -= 8;
    }
    if(mouse_x   > (SCREEN_W-10) && tile_map -> x < tile_map -> width * 64 - SCREEN_W){
       tile_map -> x += 8;
    }

    // Change selected
    if(key[KEY_UP]){
      exampleTile -> setX(0);
      exampleTile -> setY(0);
      if(exampleTile -> getType() < 400){
        do{
          exampleTile -> setType( exampleTile -> getType() + 1);
          selectedTileType = exampleTile -> getType();
        }while( exampleTile -> getImage() == NULL && exampleTile -> getType() < 400);
        rest(100);
      }
    }
    if(key[KEY_DOWN]){
      exampleTile -> setX(0);
      exampleTile -> setY(0);
      if(exampleTile -> getType() > 1){
        do{
          exampleTile -> setType( exampleTile -> getType() - 1);
          selectedTileType = exampleTile -> getType();
        }while( exampleTile -> getImage() == NULL && exampleTile -> getType() > 1);
        rest(100);
      }
    }

    // Change Layer
    if(key[KEY_TAB]){
      if(layer == 0){
        layer = 1;
      }
      else{
        layer = 0;
      }
      while(key[KEY_TAB]){}
    }

    // Place tile
    if( mouse_b & 1){
      //Check for collision
      if( layer == 1){
        for(int i = 0; i < tile_map -> mapTiles.size(); i++){
          if(collisionAny(mouse_x  + tile_map -> x, mouse_x  + tile_map -> x, tile_map -> mapTiles.at(i).getX(), tile_map -> mapTiles.at(i).getX() + 64, mouse_y + tile_map -> y, mouse_y + tile_map -> y, tile_map -> mapTiles.at(i).getY(), tile_map -> mapTiles.at(i).getY() + 64)){
            tile_map -> mapTiles.at(i).setType(selectedTileType);
          }
        }
      }
      else{
        for(int i = 0; i < tile_map -> mapTilesBack.size(); i++){
          if(collisionAny(mouse_x  + tile_map -> x, mouse_x  + tile_map -> x, tile_map -> mapTilesBack.at(i).getX(), tile_map -> mapTilesBack.at(i).getX() + 64, mouse_y + tile_map -> y, mouse_y + tile_map -> y, tile_map -> mapTilesBack.at(i).getY(), tile_map -> mapTilesBack.at(i).getY() + 64)){
            tile_map -> mapTilesBack.at(i).setType(selectedTileType);
          }
        }
      }
    }

    // Get tile type tile
    if( key[KEY_K]){
      //Check for collision
      if( layer == 1){
        for(int i = 0; i < tile_map -> mapTiles.size(); i++){
          if(collisionAny(mouse_x  + tile_map -> x, mouse_x  + tile_map -> x, tile_map -> mapTiles.at(i).getX(), tile_map -> mapTiles.at(i).getX() + 64, mouse_y + tile_map -> y, mouse_y + tile_map -> y, tile_map -> mapTiles.at(i).getY(), tile_map -> mapTiles.at(i).getY() + 64)){
            exampleTile -> setX(0);
            exampleTile -> setY(0);
            selectedTileType = tile_map -> mapTiles.at(i).getType();
            exampleTile -> setType(selectedTileType);
          }
        }
      }
      else{
        for(int i = 0; i < tile_map -> mapTilesBack.size(); i++){
          if(collisionAny(mouse_x  + tile_map -> x, mouse_x  + tile_map -> x, tile_map -> mapTilesBack.at(i).getX(), tile_map -> mapTilesBack.at(i).getX() + 64, mouse_y + tile_map -> y, mouse_y + tile_map -> y, tile_map -> mapTilesBack.at(i).getY(), tile_map -> mapTilesBack.at(i).getY() + 64)){
            exampleTile -> setX(0);
            exampleTile -> setY(0);
            selectedTileType = tile_map -> mapTilesBack.at(i).getType();
            exampleTile -> setType(selectedTileType);
          }
        }
      }
    }

    // Erase tile
    if( mouse_b & 2){
      //Check for collision
      if( layer == 1){
        for(int i = 0; i < tile_map -> mapTiles.size(); i++){
          if(collisionAny(mouse_x  + tile_map -> x, mouse_x  + tile_map -> x, tile_map -> mapTiles.at(i).getX(), tile_map -> mapTiles.at(i).getX() + 64, mouse_y + tile_map -> y, mouse_y + tile_map -> y, tile_map -> mapTiles.at(i).getY(), tile_map -> mapTiles.at(i).getY() + 64)){
            tile_map -> mapTiles.at(i).setType(0);
          }
        }
      }
      else{
        for(int i = 0; i < tile_map -> mapTilesBack.size(); i++){
          if(collisionAny(mouse_x + tile_map -> x, mouse_x  + tile_map -> x, tile_map -> mapTilesBack.at(i).getX(), tile_map -> mapTilesBack.at(i).getX() + 64, mouse_y + tile_map -> y, mouse_y + tile_map -> y, tile_map -> mapTilesBack.at(i).getY(), tile_map -> mapTilesBack.at(i).getY() + 64)){
            tile_map -> mapTilesBack.at(i).setType(0);
          }
        }
      }
    }

    // Save map
    if(key[KEY_S]){
      clear_keybuf();
      saving = true;
    }

    // Open map
    if(key[KEY_O]){
      clear_keybuf();
      opening = true;
    }

    //Fill map
    if(key[KEY_F]){
      for(int i = 0; i < tile_map -> mapTilesBack.size(); i++){
        tile_map -> mapTilesBack.at(i).setType(selectedTileType);
      }
    }
    if(key[KEY_G]){
      for(int i = 0; i < tile_map -> mapTiles.size(); i++){
        tile_map -> mapTiles.at(i).setType(selectedTileType);
      }
    }
  }
}

void Editor::draw()
{
  // Background
  rectfill(buffer, 0, 0, SCREEN_W, SCREEN_H, makecol(255,255,255));

  if( saving){
    //Create gui
    textprintf_centre_ex(buffer,font,640,310, makecol(0,0,0),-1,"Save Map Name");

    //Input rectangle
    rectfill(buffer, 400, 408, 892, 452, makecol(0,0,0));
    rectfill(buffer, 402, 410, 890, 450, makecol(255,255,255));

    // Output the string to the screen
    textout_ex(buffer, font, edittext.c_str(), 410, 410, makecol(0,0,0), -1);

    // Draw the caret
    vline(buffer, text_length(font, edittext.c_str()) + 410 - text_length(font, ".txt") , 412, 448, makecol(0,0,0));
  }
  else if( opening){
    //Create gui
    textprintf_centre_ex(buffer,font,640,310, makecol(0,0,0),-1,"Open Map Name");

    //Input rectangle
    rectfill(buffer, 400, 408, 892, 452, makecol(0,0,0));
    rectfill(buffer, 402, 410, 890, 450, makecol(255,255,255));

    // Output the string to the screen
    textout_ex(buffer, font, edittext.c_str(), 410, 410, makecol(0,0,0), -1);

    // Draw the caret
    vline(buffer, text_length(font, edittext.c_str()) + 410 - text_length(font, ".txt") , 412, 448, makecol(0,0,0));
  }
  else{
    // Draw tiles
    tile_map -> draw_map( buffer);
    exampleTile -> draw_tile( buffer, 0, 0, 0);

    // Map info
    textprintf_ex(buffer,font,0,80,makecol(255,255,255),makecol(0,0,0),"height-%i width-%i", tile_map -> height, tile_map -> width);

    if(layer == 1){
      textprintf_ex(buffer,font,0,130,makecol(255,255,255),makecol(0,0,0),"Editing Mode: Foreground");
    }
    else if(layer == 0){
      textprintf_ex(buffer,font,0,130,makecol(255,255,255),makecol(0,0,0),"Editing Mode: Background");
    }

    // Cursor
    circlefill(buffer, mouse_x , mouse_y , 10, makecol(0,0,0));
    circlefill(buffer, mouse_x , mouse_y, 8, makecol(255,255,255));
  }
  // Draw buffer
  stretch_sprite( screen, buffer, 0, 0, SCREEN_W, SCREEN_H);
}

Editor::~Editor()
{
  destroy_bitmap( buffer);

  delete tile_map;
  delete exampleTile;
}
