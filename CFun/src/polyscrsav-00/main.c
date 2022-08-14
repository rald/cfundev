#include <SDL2/SDL.h>

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

int w, h;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *display;

int main(int argc, char *args[])
{
	srand(time(NULL));

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

	int speed = 10;
	int n = 4, m = 4;
	int x[m][n], y[m][n], dx[m][n], dy[m][n];
	SDL_Color z[m];
	int c = 0;

	for (int j = 0; j < m; j++)
	{
		for (int i = 0; i < n; i++)
		{
			x[j][i] = rand() % w;
			y[j][i] = rand() % h;
			dx[j][i] = (rand() % 2 ? 1 : -1) * (rand() % speed + 10);
			dy[j][i] = (rand() % 2 ? 1 : -1) * (rand() % speed + 10);
		}

		z[j].r = rand() % 256;
		z[j].g = rand() % 256;
		z[j].b = rand() % 256;
		z[j].a = 255;
	}

	while (1)
	{
		SDL_SetRenderTarget(renderer, display);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 10);
		SDL_RenderFillRect(renderer, &(SDL_Rect){0, 0, w, h});

		c++;
		if (c == 100)
		{
			c=0;
			for (int j = 0; j < m; j++)
			{
				z[j].r = rand() % 256;
				z[j].g = rand() % 256;
				z[j].b = rand() % 256;
				z[j].a = 255;
			}
		}

		for (int j = 0; j < m; j++)
		{
			SDL_SetRenderDrawColor(renderer, z[j].r, z[j].g, z[j].b, z[j].a);

			for (int i = 0; i < n - 1; i++)
			{
				SDL_RenderDrawLine(renderer, x[j][i], y[j][i], x[j][i + 1], y[j][i + 1]);
			}
			SDL_RenderDrawLine(renderer, x[j][n - 1], y[j][n - 1], x[j][0], y[j][0]);

			for (int i = 0; i < n; i++)
			{
				x[j][i] += dx[j][i];
				y[j][i] += dy[j][i];

				if (x[j][i] < 0)
					dx[j][i] = abs(dx[j][i]);

				if (y[j][i] < 0)
					dy[j][i] = abs(dx[j][i]);

				if (x[j][i] >= w)
					dx[j][i] = -abs(dx[j][i]);

				if (y[j][i] >= h)
					dy[j][i] = -abs(dx[j][i]);
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
