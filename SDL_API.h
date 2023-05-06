#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>

using namespace std;

class Screen {
public:
	Screen(int width, int height );
	~Screen();

	void loadFont(std::string font_path, int font_size);
	void replaceFont(std::string font_path, int new_font_size);
	void update();
	void clear();
	void drawCharChain(const char* text, int x, int y, SDL_Color color);
	void drawString(const string& text, int x, int y, SDL_Color color);
	void delay(int ms);
	void setPixel(int x, int y, SDL_Color c);
	void drawLine(int x1, int y1, int x2, int y2, SDL_Color c);
	void setColor(SDL_Color c);
	void strokeRect(int x, int y, int w, int h, SDL_Color c);
	void fillRect(int x, int y, int w, int h, SDL_Color c);
	void drawImage(const string& path, int x, int y, int w = -1, int h = -1);
	void drawSurface(SDL_Surface* surface, int x, int y, int w = -1, int h = -1);
	std::pair<int, int> getWindowSize();

	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;
};

