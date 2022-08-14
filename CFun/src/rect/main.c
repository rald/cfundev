#include <SDL2/SDL.h>

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

int sz;
int sw, sh;
int w, h;
int ox, oy;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *display;

bool inrect(int x, int y, int rx, int ry, int rw, int rh)
{
	return x >= rx && x <= rx + rw && y >= ry && y <= ry + rh;
}

int main(int argc, char *args[])
{
	srand(time(NULL));

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

	ox = (w - sw) / 2;
	oy = (h - sh) / 2;

	while (1)
	{
		SDL_SetRenderTarget(renderer, display);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 0, 80, 0, 255);

		SDL_RenderFillRect(renderer, &(SDL_Rect){ox, oy, sw, sh});

		for (int k = 0; k < 16; k++)
		{
			int g = 16;
			int bw = 128;
			int bh = 128;
			int cx = (sw - ((bw + g) * 4)) / 2;
			int cy = (sh - ((bh + g) * 4)) / 2;
			int x = (k % 4) * (bw + g) + cx + ox;
			int y = (k / 4) * (bh + g) + cy + oy;

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

			SDL_RenderDrawRect(renderer, &(SDL_Rect){x, y, bw, bh});

			int numTouchDevices = SDL_GetNumTouchDevices();
			for (int j = 0; j < numTouchDevices; j++)
			{
				SDL_TouchID touchId = SDL_GetTouchDevice(j);

				int numTouchFingers = SDL_GetNumTouchFingers(touchId);

				for (int i = 0; i < numTouchFingers; i++)
				{
					SDL_Finger *finger = SDL_GetTouchFinger(touchId, i);

					if(finger) {

					int tx = finger->x * w;
					int ty = finger->y * h;

					SDL_SetRenderDrawColor(renderer, 0, 0, 80, 40);

					SDL_RenderFillRect(renderer, &(SDL_Rect){tx - 32, ty - 32, 64, 64});

					if (inrect(tx, ty, x, y, bw, bh))
					{
						SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

						SDL_RenderFillRect(renderer, &(SDL_Rect){x, y, bw, bh});
					}
					}
				}
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
