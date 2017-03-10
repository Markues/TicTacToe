#include "classes.h"
#include "globals.h"

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

void Move::setArrayPos(int col, int row) {
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
			SDL_RenderDrawRect(gRenderer, &gSpriteClips[colNum][rowNum]);
			break;
	}
}
