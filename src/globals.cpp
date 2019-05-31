#include "globals.h"

BITMAP *tileSprites[400][8];
BITMAP *projectileSprites[10][3];

// Frames
bool single_player = false;

//State variables
int stateID;
int nextState;
int levelOn;
