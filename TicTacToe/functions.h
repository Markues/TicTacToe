#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Starts up SDL and creates window
bool init();

int computePostion(int position);

// Loads media
bool loadMedia();

// Frees media and shuts down SDL
void close();

#endif
