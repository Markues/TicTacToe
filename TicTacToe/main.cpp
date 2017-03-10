#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <string>
#include "constants.h"
#include "classes.h"
#include "functions.h"
#include "globals.h"

using namespace std;

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
					for(int i = 0; i < NUM_COLS; i++) {
						for(int j = 0; j < NUM_ROWS; j++) {
							gMoves[i][j].handleEvent(&e);
						}
					}
				}
				
				// Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				board.render();
				
				// Render Moves
				for(int i = 0; i < NUM_COLS; i++) {
					for(int j = 0; j < NUM_ROWS; j++) {
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
