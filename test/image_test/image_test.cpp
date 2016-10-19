#include <iostream>

#include "SDL.h"

#define SDL_STB_IMAGE_IMPLEMENTATION
#include "SDL_stb_image.hpp"

namespace {

constexpr int kWindowWidth = 640;
constexpr int kWindowHeight = 480;

void printError()
{
	std::cerr << SDL_GetError() << std::endl;
}

} // namespace

int main(int argc, char* argv[])
{
	int result = 0;

	// init system
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		printError();
		result = 1;

	} else {
		// create window
		auto window = SDL_CreateWindow("image_test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ::kWindowWidth, ::kWindowHeight, 0);
		if (window == nullptr)
		{
			printError();
			result = 1;
			goto INITED_SYSTEM;
		}

		// create renderer
		auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == nullptr)
		{
			printError();
			result = 1;
			goto CREATED_WINDOW;
		}

		// create surface
		//auto surface = SDL_LoadBMP("test.bmp");
		auto surface = STB_IMG_Load("test.png");
		if (surface == nullptr)
		{
			printError();
			result = 1;
			goto CREATED_RENDERER;
		}

		// create texture
		auto texture = SDL_CreateTextureFromSurface(renderer, surface);

		// free surface
		SDL_FreeSurface(surface);
		surface = nullptr;

		// main loop
		while (1)
		{
			SDL_Event event;
			if (SDL_PollEvent(&event) != 0) {
				if (event.type == SDL_QUIT) {
					break;
				}
			}

			SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, 0xFF);
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, nullptr, nullptr);
			SDL_RenderPresent(renderer);
		}

CREATED_TEXTURE:
		SDL_DestroyTexture(texture);

CREATED_RENDERER:
		SDL_DestroyRenderer(renderer);

CREATED_WINDOW:
		SDL_DestroyWindow(window);

INITED_SYSTEM:
		SDL_Quit();
	}

	return result;
}