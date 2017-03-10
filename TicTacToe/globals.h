#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL2/SDL.h>
#include "classes.h"

// The window we'll be rendering to
extern SDL_Window* gWindow;

// The window renderer
extern SDL_Renderer* gRenderer;

// Scene textures
extern LTexture gBoardTexture;
extern LTexture gXTexture;
extern LTexture gOTexture;

// Mouse button sprites
extern SDL_Rect gSpriteClips[NUM_ROWS][NUM_COLS];

// Buttons objects
extern Move gMoves[NUM_ROWS][NUM_COLS];

extern bool isXTurn;

#endif
