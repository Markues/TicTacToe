#include "classes.h"
#include "constants.h"

// The window we'll be rendering to
SDL_Window* gWindow = NULL;

// The window renderer
SDL_Renderer* gRenderer = NULL;

// Scene textures
LTexture gBoardTexture;
LTexture gXTexture;
LTexture gOTexture;

// Mouse button sprites
SDL_Rect gSpriteClips[NUM_COLS][NUM_ROWS];

// Buttons objects
Move gMoves[NUM_COLS][NUM_ROWS];

bool isXTurn = true;
