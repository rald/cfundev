#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "palette.h"

#define GRAPHICS_IMPLEMENTATION_H
#include "graphics.h"

#define UTIL_IMPLEMENTATION_H
#include "util.h"

#define BUTTON_IMPLEMENTATION_H
#include "button.h"

int x,y;
int i,j;
int w, h;
int sw = 160, sh = 144;
int cw = 16, ch = 16, cs = 8;
int ox = 0, oy = 0;
int *px=NULL;

static SDL_Texture *display;
static SDL_Event event;
SDL_Point touch;
bool touchDown = false;
int selectedButtonIndex = -1;

int main(int argc, char *args[])
{
	bool quit = false;

	w = sw;
	h = sh;

	px=calloc(cw*ch,sizeof(*px));

	srand(time(NULL));

	SDL_SetHint(SDL_HINT_ORIENTATIONS, "Portrait");

	SDL_Init(SDL_INIT_VIDEO);

	SDL_CreateWindowAndRenderer(w, h, 0, &window, &renderer);
	SDL_GetWindowSize(window, &w, &h);

	ox = (w - sw * pixelSize) / pixelSize / 2;
	oy = (h - sh * pixelSize) / pixelSize / 2;

	display = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

	SDL_SetRenderTarget(renderer, display);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	SetDrawColor(renderer, palette[4]);
	FillRect(renderer, ox, oy, sw, sh);

	button_t *colorButton[5] = {
		CreateButton(sw - 16 + ox,  0 + oy, 16, 16, palette[0]),
		CreateButton(sw - 16 + ox, 16 + oy, 16, 16, palette[1]),
		CreateButton(sw - 16 + ox, 32 + oy, 16, 16, palette[2]),
		CreateButton(sw - 16 + ox, 48 + oy, 16, 16, palette[3]),
		CreateButton(sw - 16 + ox, 64 + oy, 16, 16, palette[4]),
	};

	for (int j = 0; j < ch; j++)
	{
		for (int i = 0; i < cw; i++)
		{
			SetDrawColor(renderer, palette[4]);
			FillRect(renderer, i * cs + ox, j * cs + oy, cs, cs);
			SetDrawColor(renderer, palette[0]);
			DrawRect(renderer, i * cs + ox, j * cs + oy, cs, cs);
		}
	}

	touch.x = w / 2;
	touch.y = h / 2;

	while (!quit)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (event.type == SDL_WINDOWEVENT)
			{
				if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					w = event.window.data1;
					h = event.window.data2;

					SDL_RenderPresent(renderer);
				}
			}
			else if (event.type == SDL_FINGERDOWN)
			{
				touch.x = event.tfinger.x * w;
				touch.y = event.tfinger.y * h;
				touchDown = true;
			}
			else if (event.type == SDL_FINGERMOTION)
			{
				touch.x = event.tfinger.x * w;
				touch.y = event.tfinger.y * h;
				touchDown = true;
			}
			else if (event.type == SDL_FINGERUP)
			{
				touch.x = event.tfinger.x * w;
				touch.y = event.tfinger.y * h;
				touchDown = false;
			}
		}

		SDL_SetRenderTarget(renderer, display);

		for (int i = 0; i < 5; i++)
		{
			if (i == selectedButtonIndex)
			{
				colorButton[i]->isDown = true;
			}
			else
			{
				colorButton[i]->isDown = false;
			}
			DrawButton(renderer, colorButton[i]);
		}

		for (int i = 0; i < 5; i++)
		{
			if (HandleButtonEvents(colorButton[i], touch))
			{
				selectedButtonIndex = i;
			}
		}
									
		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, display, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(display);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
