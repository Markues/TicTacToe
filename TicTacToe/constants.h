#ifndef CONSTANTS_H
#define CONSTANTS_H

// Screen dimension constants
const int SCREEN_WIDTH = 180;
const int SCREEN_HEIGHT = 180;
const int SCREEN_BORDER_SIZE = 20;
const int MOVE_DIMENSION = 40;
const int MOVE_OUTER_BORDER_SIZE = 2;
const int MOVE_INNER_BORDER_SIZE = 6;
const int NUM_ROWS = 3;
const int NUM_COLS = 3;

enum MoveSprite {
	X_SPRITE = 0,
	O_SPRITE = 1,
	NO_SPRITE = 2
};

#endif
