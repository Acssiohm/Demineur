#include <iostream>
#include "SDL_API.h"
#include "game.h"
#include "game_renderer.h"

int main(int argc, char ** argv){
	// Plateau p(20, 20);
	// std::cout << g << std::endl;

	Screen screen(600, 400);
	GamePlayer gp(20, 20, &screen);
	gp.show();
	screen.update();
	SDL_bool run = SDL_TRUE;
	while (run) {
		SDL_Event event;
		screen.update();
		screen.delay(100);
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				run = SDL_FALSE;
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					screen.clear();
					gp.show();
				}
				break;
			// case SDL_KEYUP:
			default:
				break;

			}
		}
	}

	return 0;
}