#ifndef CLASSES_H
#define CLASSES_H

#include <string>
#include <SDL2/SDL.h>
#include "constants.h"

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
	void setArrayPos(int col, int row);
	
	void handleEvent(SDL_Event* e);
	
	void render();
	
private:
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

#endif
