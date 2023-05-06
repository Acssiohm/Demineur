#include <iostream>
#include "SDL_API.h"
#include "game.h"

int main(int argc, char ** argv){
	// Plateau p(20, 20);
	GamePlayer gp(20, 20);
	// std::cout << g << std::endl;

	Screen screen(600, 400);
	gp.show(screen);
	screen.update();
	SDL_bool run = SDL_TRUE;
	while (run) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				run = SDL_FALSE;
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					auto [new_width, new_height] = screen.getWindowSize();
					screen.clear();
					gp.show(screen);
					screen.update();
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