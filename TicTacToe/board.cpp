#include "classes.h"
#include "globals.h"

Board::Board() {
	xPos = SCREEN_BORDER_SIZE;
	yPos = SCREEN_BORDER_SIZE;
}

void Board::render() {
	gBoardTexture.render(xPos, yPos);
}
