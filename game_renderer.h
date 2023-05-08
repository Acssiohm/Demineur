#pragma once
#include "SDL_API.h"
#include "game.h"
#include <assert.h>
SDL_Texture * getTexture(const std::string& image_file, Screen * screen);

class GamePlayer{
	Game m_game;
	bool is_first_move;
	Screen * m_screen;
	
	SDL_Texture 
		* flag_texture, 
		* void_texture, 
		* bomb_texture, 
		* back_texture;

	class FontHandler{
		std::vector<TTF_Font *> m_fonts;
		// std::vector<bool> m_fonts_av;
		const std::string m_font_file;
	public:
		FontHandler(const std::string& font_file, int prepare = 0): m_font_file(font_file){
			// m_fonts_av.push_back(false);
			m_fonts.push_back(nullptr);
			for (int i = 1; i < prepare; ++i){
				get_font(i);
			}
		}
		TTF_Font* get_font(int font_size){
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

	};

	FontHandler m_font_handler;
public:
	GamePlayer(int size, int nb_bombs, Screen * screen);
	bool isMoveAllowed(int x, int y);
	bool play(int x, int y);
	void show();
};