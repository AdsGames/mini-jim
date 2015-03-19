#include "globals.h"

BITMAP* tileSprites[400][8];
BITMAP* projectileSprites[10][3];

// Frames
int frames_done;
bool single_player = false;

// Resdiv
int resDiv;

//State variables
int stateID;
int nextState;
int levelOn;
