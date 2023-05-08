#include "game_renderer.h"


SDL_Texture * getTexture(const std::string& image_file, Screen * screen){
	// std::cout << image_file << " BEG" << std::endl;
	SDL_Surface* surface = IMG_Load(image_file.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(screen -> renderer, surface);
	SDL_FreeSurface(surface);
	if(texture == nullptr){
		std::cout << "NULLPTR !!" << std::endl;
	}
	// std::cout << image_file << " END" << std::endl;
	return texture ;
}

GamePlayer::GamePlayer(int size, int nb_bombs, Screen * screen): 
	m_game(size, nb_bombs), 
	is_first_move(true), 
	m_font_handler("font.ttf", 30), 
	m_screen(screen),
	flag_texture(getTexture("case_flag.png", screen)), 
	void_texture(getTexture("case_vide.png", screen)), 
	bomb_texture(getTexture("case_bombe.png", screen)), 
	back_texture(getTexture("case_back.png", screen))
	{}

bool GamePlayer::isMoveAllowed(int x, int y){
	return m_game.is_in_grille(x, y) && !m_game.flagged(x, y) && !m_game.revealed(x, y);
}
bool GamePlayer::play(int x, int y){
	if(!isMoveAllowed(x, y)){
		return true;
	}
	m_game.reveal(x, y, is_first_move);
	is_first_move = false;
	return m_game.isBomb(x, y);
}

void GamePlayer::show(){
	assert(m_screen != nullptr);
	auto [w, h] = m_screen -> getWindowSize();
	int s_w = w / m_game.get_size();
	int s_h = h / m_game.get_size();
	int s_size = std::min(s_w, s_h);
	int text_size = 2*s_size/3;
	m_screen -> clear();
	TTF_Font * res = m_font_handler.get_font(text_size);
	// std::cout << text_size << std::endl;
	m_screen -> setFont( res );
	for (int y = 0; y < m_game.get_size(); ++y) {
		for (int x = 0; x < m_game.get_size(); ++x) {
			int x_pos = x*s_size;
			int y_pos = y*s_size;
			// std::cout << "	HEY 2.1 :" << x << "," <<y << std::endl;
			if(m_game.flagged(x, y)){
				m_screen -> drawTexture(flag_texture, x_pos, y_pos, s_size, s_size);
			} else if(!m_game.revealed(x, y)){
				m_screen -> drawTexture(void_texture, x_pos, y_pos, s_size, s_size);
			} else if(m_game.isBomb(x, y)){
				m_screen -> drawTexture(bomb_texture, x_pos, y_pos, s_size, s_size);
			} else {
				m_screen -> drawTexture(back_texture, x_pos, y_pos, s_size, s_size);
				SDL_Color c = {.r = 200, .g=100, .b=0, .a=255};
				m_screen -> drawString(std::to_string(m_game.get(x, y)), x_pos + (s_size - text_size)/2, y_pos + (s_size - text_size)/2, c);
			}
			// std::cout << "	HEY 2.2 :" << x << "," <<y << std::endl;
		}
	}

}