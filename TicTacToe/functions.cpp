#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "constants.h"
#include "globals.h"

bool init() {
	// Initialization flag
	bool success = true;
	
	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		// Set texture filtering to linear
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
		}
		
		// Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			// Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if(gRenderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				// Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				
				// Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}
	
	return success;
}

int computePostion(int position) {
	return SCREEN_BORDER_SIZE + (position * MOVE_DIMENSION) + ((position + 1) * MOVE_OUTER_BORDER_SIZE) + (position * MOVE_INNER_BORDER_SIZE);
}

bool loadMedia() {
	// Loading success flag
	bool success = true;
	
	if(!gBoardTexture.loadFromFile("Board.png")) {
		printf("Failed to load board texture!\n");
		success = false;
	}
	else if(!gXTexture.loadFromFile("X.png")) {
		printf("Failed to load X texture!\n");
		success = false;
	}
	else if(!gOTexture.loadFromFile("O.png")) {
		printf("Failed to load O texture!\n");
		success = false;
	}
	else {
		for(int i = 0; i < NUM_COLS; i++) {
			for(int j = 0; j < NUM_ROWS; j++) {
				gMoves[i][j].setPosition(computePostion(i), computePostion(j));
				gMoves[i][j].setArrayPos(i, j);
				gSpriteClips[i][j].x = computePostion(i);
				gSpriteClips[i][j].y = computePostion(j);
				gSpriteClips[i][j].w = MOVE_DIMENSION;
				gSpriteClips[i][j].h = MOVE_DIMENSION;
			}
		}
	}
	
	return success;
}

void close() {
	// Free loaded images
	gBoardTexture.free();
	gXTexture.free();
	gOTexture.free();
	
	// Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	
	// Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
