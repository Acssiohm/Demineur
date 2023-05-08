#include <iostream>
#include "SDL_API.h"
#include "game.h"
#include "game_renderer.h"

int main(int argc, char ** argv){
	// Plateau p(20, 20);
	// std::cout << g << std::endl;

	Screen screen(600, 400);
	GamePlayer gp(20, 20, &screen);
	SDL_bool run = SDL_TRUE;
	bool do_update = true;
	while (run) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				run = SDL_FALSE;
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					gp.process_changed_win_size();
					do_update = true;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					gp.process_left_click( event.button.x, event.button.y );
					do_update = true;
				} else if (event.button.button == SDL_BUTTON_RIGHT) {
					gp.process_right_click( event.button.x, event.button.y );
					do_update = true;
				}
				break;
			default:
				break;
			}
		}
		if(do_update){
			screen.clear();
			gp.show();
			screen.update();
			do_update = false;
		} else {
			screen.delay(100);
		}
	}

	return 0;
}