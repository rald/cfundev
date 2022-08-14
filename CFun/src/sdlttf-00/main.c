#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "sweetie16.h"

int fps = 60;

int sc;
int sw, sh;
int w, h;
int ox, oy;
int ps;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *display;

char txt[256];
int ctr = 0;

int main(void)
{
	srand(time(NULL));

	sc = 2;
	ps = 8;

	sw = 360 * sc;
	sh = 646 * sc;
	w = sw;
	h = sh;

	SDL_Init(SDL_INIT_VIDEO);

	TTF_Init();

	TTF_Font *font = TTF_OpenFont("lazy.ttf", 28 * sc);

	SDL_CreateWindowAndRenderer(w, h, 0, &window, &renderer);
	SDL_GetWindowSize(window, &w, &h);

	ox = (w - sw) / 2;
	oy = (h - sh) / 2;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	display = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

	while (1)
	{
		SDL_SetRenderTarget(renderer, display);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		sprintf(txt, "%07d", ctr++);

		SDL_Surface *txtsrf = TTF_RenderText_Solid(font, txt, (SDL_Color){sweetie16[4][0], sweetie16[4][1], sweetie16[4][2], sweetie16[4][3]});

		SDL_Texture *txttex = SDL_CreateTextureFromSurface(renderer, txtsrf);

		SDL_RenderCopyEx(renderer, txttex, NULL, &(SDL_Rect){ox, oy, txtsrf->w, txtsrf->h}, 0, &(SDL_Point){0, 0}, SDL_FLIP_NONE);

		SDL_FreeSurface(txtsrf);
		SDL_DestroyTexture(txttex);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, &(SDL_Rect){ox, oy, sw, sh});

		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, display, NULL, NULL);
		SDL_RenderPresent(renderer);
		
		SDL_Delay(1000/fps);
	}

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}