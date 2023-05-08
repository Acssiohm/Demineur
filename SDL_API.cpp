#include "SDL_API.h"
#include <assert.h>
#include <iostream>
Screen::Screen(int width, int height) :window(nullptr), renderer(nullptr),font(nullptr) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
	TTF_Init();
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderPresent(renderer);
}
void Screen::setFont(TTF_Font * f){
	font = f;
}
void Screen::update() {
	SDL_RenderPresent(renderer);
}
void Screen::clear() {
	SDL_RenderClear(renderer);
}
void Screen::drawCharChain(const char* text, int x, int y, SDL_Color color) {
	assert(font != nullptr);
	assert(text != nullptr);
	// std::cout << "	HEY 2.1.3 " << text << std::endl;
	SDL_Surface * text_surface = TTF_RenderText_Solid(font, text, color);
	// std::cout << "	HEY 2.1.3"<< std::endl;
	SDL_Texture * text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_Rect text_rect;
	SDL_QueryTexture(text_texture, nullptr, nullptr, &text_rect.w, &text_rect.h);
	text_rect.x = x;
	text_rect.y = y;
	SDL_RenderCopy(renderer, text_texture, nullptr, &text_rect);
	SDL_FreeSurface(text_surface);
}
void Screen::drawString(const string& text, int x, int y, SDL_Color color) {
	drawCharChain(text.c_str(), x, y, color);
}
void Screen::delay(int ms) {
	SDL_Delay(ms);
}
// void Screen::setPixel(int x, int y, SDL_Color c) {
// 	setColor(c);
// 	SDL_RenderDrawPoint(renderer, x, y);
// }
// void Screen::drawLine(int x1, int y1, int x2, int y2, SDL_Color c) {
// 	setColor(c);
// 	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
// }
void Screen::setColor(SDL_Color c){
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
}
// void Screen::strokeRect(int x, int y, int w, int h, SDL_Color c) {
// 	setColor(c);
// 	SDL_Rect rect;
// 	rect.x = x;
// 	rect.y = y;
// 	rect.w = w;
// 	rect.h = h;
// 	SDL_RenderDrawRect(renderer, &rect);
// }
// void Screen::fillRect(int x, int y, int w, int h, SDL_Color c) {
// 	setColor(c);
// 	SDL_Rect rect;
// 	rect.x = x;
// 	rect.y = y;
// 	rect.w = w;
// 	rect.h = h;
// 	SDL_RenderFillRect(renderer, &rect);
// }
std::pair<int, int> Screen::getWindowSize(){
	int win_width;
	int win_height;
	SDL_GetWindowSize(window, &win_width, &win_height);
	return {win_width, win_height};
}
// void Screen::drawSurface(SDL_Surface* surface, int x, int y, int w, int h) {
// 	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
// 	SDL_Rect dest_rect;
// 	dest_rect.x = x;
// 	dest_rect.y = y;
// 	dest_rect.w = (w == -1) ? surface->w : w;
// 	dest_rect.h = (h == -1) ? (surface->h * dest_rect.w) / surface->w : h;
// 	SDL_RenderCopy(renderer, texture, NULL, &dest_rect);
// 	SDL_DestroyTexture(texture);
// }
void Screen::drawTexture(SDL_Texture* texture, int x, int y, int w, int h) {
	// std::cout << "	HEY 2.1.1 *"<< std::endl;
	assert(texture != nullptr);
	SDL_Rect dest_rect;
	dest_rect.x = x;
	dest_rect.y = y;
	dest_rect.w = w;
	dest_rect.h = h;
	SDL_RenderCopy(renderer, texture, NULL, &dest_rect);
	// std::cout << "	HEY 2.1.2"<< std::endl;
}
// void Screen::drawImage(const string& path, int x, int y, int w, int h) {
// 	SDL_Surface* surface = IMG_Load(path.c_str());
// 	drawSurface(surface, x, y, w, h);
// 	SDL_FreeSurface(surface);
// }

Screen::~Screen() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	IMG_Quit();
}
