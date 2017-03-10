//
//  Created by Jeff Farris on 3/4/17.
//  Copyright © 2017 Markues. All rights reserved.
//

// Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <stdio.h>
#include <string>

// Screen dimension constants
const int SCREEN_WIDTH = 180;
const int SCREEN_HEIGHT = 180;
const int SCREEN_BORDER_SIZE = 20;
const int MOVE_DIMENSION = 40;
const int MOVE_OUTER_BORDER_SIZE = 2;
const int MOVE_INNER_BORDER_SIZE = 6;
const int NUM_ROWS = 3;
const int NUM_COLS = 3;

bool isXTurn = true;

enum MoveSprite {
	X_SPRITE = 0,
	O_SPRITE = 1,
	NO_SPRITE = 2
};

using namespace std;

// Texture wrapper class
class LTexture {
	public:
		// Initializes variables
		LTexture();
		
		// Deallocates memory
		~LTexture();
		
		// Loads image at specified path
		bool loadFromFile(string path);
		
	#ifdef _SDL_TTF_H
		// Creates image from font string
		bool loadFromRenderedText(string textureText, SDL_Color textColor);
	#endif
		
		// Deallocates texture
		void free();
		
		// Set color modulation
		void setColor(Uint8 red, Uint8 green, Uint8 blue);
		
		// Set blending
		void setBlendMode(SDL_BlendMode blending);
		
		// Set alpha modulation
		void setAlpha(Uint8 alpha);
		
		// Renders texture at given point
		void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
		
		// Gets image dimensions
		int getWidth();
		int getHeight();
		
	private:
		// The actual hardware texture
		SDL_Texture* mTexture;
		
		// Image dimensions
		int mWidth;
		int mHeight;
	};

class Move {
	public:
		Move();
	
		void setPosition(int x, int y);
		void setArrayPos(int row, int col);
	
		void handleEvent(SDL_Event* e);
	 
		void render();
		
	private:
		// Top left position
		SDL_Point mPosition;
	
		int rowNum;
		int colNum;
	
		bool isClicked;
	 
		// Currently used global sprite
		MoveSprite mCurrentSprite;
};

class Board {
	public:
		static const int BOARD_DIMENSION = 140;
	
		Board();
	
		void render();
	
	private:
		int xPos, yPos;
};

// Starts up SDL and creates window
bool init();

// Loads media
bool loadMedia();

// Frees media and shuts down SDL
void close();

// The window we'll be rendering to
SDL_Window* gWindow = NULL;

// The window renderer
SDL_Renderer* gRenderer = NULL;

// Scene textures
LTexture gBoardTexture;
LTexture gXTexture;
LTexture gOTexture;

// Mouse button sprites
SDL_Rect gSpriteClips[NUM_ROWS][NUM_COLS];

// Buttons objects
Move gMoves[NUM_ROWS][NUM_COLS];

LTexture::LTexture() {
	// Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture() {
	// Deallocate
	free();
}

bool LTexture::loadFromFile(string path) {
	// Get rid of preexisting texture
	free();
	
	// The final texture
	SDL_Texture* newTexture = NULL;
	
	// Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if(loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else {
		// Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
		
		// Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if(newTexture == NULL) {
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else {
			// Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		
		// Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}
	
	// Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText(string textureText, SDL_Color textColor) {
	// Get rid of preexisting texture
	free();
	
	// Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if(textSurface != NULL) {
		// Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if(mTexture == NULL) {
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else {
			// Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}
		
		// Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	else {
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	
	// Return success
	return mTexture != NULL;
}
#endif

void LTexture::free() {
	// Free texture if it exists
	if(mTexture != NULL) {
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	// Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending) {
	// Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha) {
	// Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	// Set rendering space and render to screen
	SDL_Rect renderQuad = {x, y, mWidth, mHeight};
	
	// Set clip rendering dimensions
	if(clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	
	// Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth() {
	return mWidth;
}

int LTexture::getHeight() {
	return mHeight;
}

Board::Board() {
	xPos = SCREEN_BORDER_SIZE;
	yPos = SCREEN_BORDER_SIZE;
}

void Board::render() {
	gBoardTexture.render(xPos, yPos);
}

Move::Move() {
	mPosition.x = 0;
	mPosition.y = 0;
	rowNum = 0;
	colNum = 0;
	
	isClicked = false;
	mCurrentSprite = NO_SPRITE;
}

void Move::setPosition(int x, int y) {
	mPosition.x = x;
	mPosition.y = y;
}

void Move::setArrayPos(int row, int col) {
	rowNum = row;
	colNum = col;
}

void Move::handleEvent(SDL_Event* e) {
	if(e->type == SDL_MOUSEBUTTONDOWN) {
		// Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);
		
		// Check if mouse is in button
		bool inside = true;
		
		if(x < mPosition.x || x > mPosition.x + MOVE_DIMENSION || y < mPosition.y || y > mPosition.y + MOVE_DIMENSION) {
			inside = false;
		}
		
		if(inside) {
			if(!isClicked) {
				if(isXTurn) {
					mCurrentSprite = X_SPRITE;
				}
				else {
					mCurrentSprite = O_SPRITE;
				}
				isClicked = true;
				isXTurn = !isXTurn;
			}
		}
	}
}

void Move::render() {
	switch (mCurrentSprite) {
		case X_SPRITE:
			gXTexture.render(mPosition.x, mPosition.y);
			break;
		case O_SPRITE:
			gOTexture.render(mPosition.x, mPosition.y);
			break;
		case NO_SPRITE:
			SDL_RenderDrawRect(gRenderer, &gSpriteClips[rowNum][colNum]);
			break;
		}
}

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
		for(int i = 0; i < NUM_ROWS; i++) {
			for(int j = 0; j < NUM_COLS; j++) {
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

int main(int argc, char* args[]) {
	// Start up SDL and create window
	if(!init()) {
		printf("Failed to initialize!\n");
	}
	else {
		// Load media
		if(!loadMedia()) {
			printf("Failed to load media!\n");
		}
		else {
			// Main loop flag
			bool quit = false;
			
			// Event handler
			SDL_Event e;

			Board board;
			
			// While application is running
			while(!quit) {
				// Handle events on queue
				while(SDL_PollEvent(&e) != 0) {
					// User requests quit
					if(e.type == SDL_QUIT) {
						quit = true;
					}
					
					// Handle button events
					for(int i = 0; i < NUM_ROWS; i++) {
						for(int j = 0; j < NUM_COLS; j++) {
							gMoves[i][j].handleEvent(&e);
						}
					}
				}
				
				// Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				board.render();
				
				// Render Moves
				for(int i = 0; i < NUM_ROWS; i++) {
					for(int j = 0; j < NUM_COLS; j++) {
						gMoves[i][j].render();
					}
				}
				
				// Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	
	// Free resources and close SDL
	close();
	
	return 0;
}
