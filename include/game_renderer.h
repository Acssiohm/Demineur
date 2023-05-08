#pragma once
#include "SDL_API.h"
#include "game.h"
#include <assert.h>
#include <array>
SDL_Texture * getTexture(const std::string& image_file, Screen * screen);

class GamePlayer{
	constexpr static std::array<SDL_Color, 8> number_colors = {{
		{.r = 0  , .g=0  , .b=255, .a=255}, // 1 : 
		{.r = 0  , .g=128, .b=0  , .a=255}, // 2 : 
		{.r = 255, .g=0  , .b=0  , .a=255}, // 3 : 
		{.r = 0  , .g=0  , .b=128, .a=255}, // 4 : 
		{.r = 128, .g=0  , .b=0  , .a=255}, // 5 : 
		{.r = 200, .g=100, .b=0  , .a=255}, // 6 : 
		{.r = 200, .g=100, .b=0  , .a=255}, // 7 : 
		{.r = 200, .g=100, .b=0  , .a=255}  // 8 : 
	}};
	class FontHandler{
		std::vector<TTF_Font *> m_fonts;
		// std::vector<bool> m_fonts_av;
		const std::string m_font_file;
	public:
		FontHandler(const std::string& font_file, int prepare = 0);
		TTF_Font* get_font(size_t font_size);
	};
	
	Game m_game;
	bool is_first_move;
	Screen * m_screen;
	int square_size;
	SDL_Texture 
		* flag_texture, 
		* void_texture, 
		* bomb_texture, 
		* back_texture;
	FontHandler m_font_handler;
public:
	GamePlayer(int size, int nb_bombs, Screen * screen);
	bool isMoveAllowed(int x, int y);
	void process_left_click(int x_pos, int y_pos);
	void process_right_click(int x_pos, int y_pos);
	void process_changed_win_size();
	bool play(int x, int y);
	void show();
};