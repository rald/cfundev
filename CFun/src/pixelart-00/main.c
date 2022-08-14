#include <SDL2/SDL.h>

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#include "sweetie16.h"
#include "bitmap00.h"

int fps = 12;

int sz;
int sw, sh;
int w, h;
int x, y, r;
int ox, oy;
int dx, dy;
int ps;

int x, y;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *display;

int bitmap00_frame = 0;

void DrawBitmap(int x, int y, int *bitmap_pixels, int bitmap_width, int bitmap_height, int frame, int pixelSize, int palette[][4])
{
	for (int j = 0; j < bitmap_height; j++)
	{
		for (int i = 0; i < bitmap_width; i++)
		{
			int p = bitmap_pixels[j * bitmap_width + i + frame * bitmap_width * bitmap_height];

			if (p != -1)
			{
				int r = palette[p][0];
				int g = palette[p][1];
				int b = palette[p][2];
				int a = palette[p][3];

				SDL_SetRenderDrawColor(renderer, r, g, b, a);
				SDL_RenderFillRect(renderer, &(SDL_Rect){i * pixelSize + x + ox, j * pixelSize + y + oy, pixelSize, pixelSize});
			}
		}
	}
}

int main(int argc, char *args[])
{
	srand(time(NULL));

	ps = 8;

	sz = 2;
	sw = 360 * sz;
	sh = 646 * sz;
	w = sw;
	h = sh;

	SDL_Init(SDL_INIT_VIDEO);

	SDL_CreateWindowAndRenderer(w, h, 0, &window, &renderer);
	SDL_GetWindowSize(window, &w, &h);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	display = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

	SDL_SetRenderTarget(renderer, display);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	SDL_RenderClear(renderer);

	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, display, NULL, NULL);
	SDL_RenderPresent(renderer);

	ox = (w - sw) / 2;
	oy = (h - sh) / 2;

	x = 0;
	y = 0;

	while (1)
	{
		SDL_SetRenderTarget(renderer, display);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, &(SDL_Rect){ox, oy, sw, sh});

		DrawBitmap(x, y, bitmap00_pixels, bitmap00_width, bitmap00_height, bitmap00_frame, ps, sweetie16);

		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, display, NULL, NULL);
		SDL_RenderPresent(renderer);

		SDL_Delay(1000 / fps);

		x = (x + 16) % (sw - 16 * ps);

		bitmap00_frame = (bitmap00_frame + 1) % bitmap00_frames;
	}


SDL_DestroyTexture(display);
SDL_DestroyWindow(window);

SDL_Quit();

return 0;
}
