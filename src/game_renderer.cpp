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



GamePlayer::FontHandler::FontHandler(const std::string& font_file, int prepare): m_font_file(font_file){
	// m_fonts_av.push_back(false);
	// m_fonts.push_back(nullptr);
	for (int i = 0; i < prepare; ++i){
		get_font(i);
	}
}
TTF_Font* GamePlayer::FontHandler::get_font(int font_size){
	while(m_fonts.size() <= font_size){
		m_fonts.push_back(nullptr);
		// m_fonts_av.push_back(false);
	}
	// assert( m_fonts.size() > font_size );
	// assert( m_fonts_av.size() > font_size );
	if(m_fonts[font_size] == nullptr){
		// std::cout << "NEW !" << std::endl;
		m_fonts[font_size] = TTF_OpenFont(m_font_file.c_str(), font_size);
		// m_fonts_av[font_size] = true;
	}
	// assert(m_fonts[font_size] != nullptr);
	// assert(m_fonts_av[font_size] == true);
	return m_fonts[font_size];
}

GamePlayer::GamePlayer(int size, int nb_bombs, Screen * screen): 
	m_game(size, nb_bombs), 
	is_first_move(true), 
	m_font_handler("Fonts/font.ttf", 30), 
	m_screen(screen),
	square_size(-1),
	flag_texture(getTexture("Images/case_flag.png", screen)), 
	void_texture(getTexture("Images/case_vide.png", screen)), 
	bomb_texture(getTexture("Images/case_bombe.png", screen)), 
	back_texture(getTexture("Images/case_back.png", screen)){
		process_changed_win_size();
		// m_game.reset_modified();
	}

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
void GamePlayer::process_changed_win_size(){
	auto [new_w, new_h] = m_screen -> getWindowSize();
	int s_w = new_w / m_game.get_size();
	int s_h = new_h / m_game.get_size();
	if( square_size != std::min(s_w, s_h) ){
		square_size = std::min(s_w, s_h);
		// m_game.reset_modified();
	}
}

void GamePlayer::show(){
	for (int x = 0; x < m_game.get_size(); x++){
		for (int y = 0; y < m_game.get_size(); y++){
			const int x_pos = x*square_size;
			const int y_pos = y*square_size;
			// std::cout << "	HEY 2.1 :" << x << "," <<y << std::endl;
			if(m_game.flagged(x, y)){
				m_screen -> drawTexture(flag_texture, x_pos, y_pos, square_size, square_size);
			} else if(!m_game.revealed(x, y)){
				m_screen -> drawTexture(void_texture, x_pos, y_pos, square_size, square_size);
			} else if(m_game.isBomb(x, y)){
				m_screen -> drawTexture(bomb_texture, x_pos, y_pos, square_size, square_size);
			} else {
				m_screen -> drawTexture(back_texture, x_pos, y_pos, square_size, square_size);
				if(m_game.get(x, y) > 0){
					int text_size = 2*square_size/3;
					m_screen -> setFont( m_font_handler.get_font(text_size) );
					m_screen -> drawString(
						std::to_string(m_game.get(x, y)), 
						x_pos + (square_size - text_size)/2, 
						y_pos + (square_size - text_size)/2, 
						number_colors[m_game.get(x, y) - 1]
						);
				}
			}
		}
	}
}
void GamePlayer::process_left_click(int x_pos, int y_pos){
	play(
		x_pos/ square_size, 
		y_pos/ square_size
		);
}
void GamePlayer::process_right_click(int x_pos, int y_pos){
	m_game.toggle_flag(
		x_pos/ square_size, 
		y_pos/ square_size
		);	
}
