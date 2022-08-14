#include <SDL2/SDL.h>

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

int sz;
int sw, sh;
int w, h;
int ox, oy;
int cx, cy;
int r;
int g;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *display;


int b[][4] = {
	{0, 0, 0, 0},
	{1, 0, 0, 0},
	{0, 1, 0, 0},
	{0, 0, 1, 0},
	{0, 0, 0, 1},
	{1, 1, 0, 0},
	{0, 1, 1, 0},
	{0, 0, 1, 1},
	{1, 0, 0, 1},
	{1, 0, 1, 0},
	{0, 1, 0, 1},
	{1, 1, 1, 1},
};

int dx[4] =
	{-1, +1, +1, -1};

int dy[4] =
	{-1, -1, +1, +1};

void DrawPoint(SDL_Renderer *renderer, int x, int y)
{
	SDL_RenderDrawPoint(renderer, x, y);
}

void DrawCircle(SDL_Renderer *renderer, int x0, int y0, int radius)
{
	int f = 1 - radius;
	int ddF_x = 0;
	int ddF_y = -2 * radius;
	int x = 0;
	int y = radius;

	DrawPoint(renderer, x0, y0 + radius);
	DrawPoint(renderer, x0, y0 - radius);
	DrawPoint(renderer, x0 + radius, y0);
	DrawPoint(renderer, x0 - radius, y0);

	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x + 1;
		DrawPoint(renderer, x0 + x, y0 + y);
		DrawPoint(renderer, x0 - x, y0 + y);
		DrawPoint(renderer, x0 + x, y0 - y);
		DrawPoint(renderer, x0 - x, y0 - y);
		DrawPoint(renderer, x0 + y, y0 + x);
		DrawPoint(renderer, x0 - y, y0 + x);
		DrawPoint(renderer, x0 + y, y0 - x);
		DrawPoint(renderer, x0 - y, y0 - x);
	}
}

void FillCircle(SDL_Renderer *renderer, int x, int y, int r)
{
	int r2 = r * r;
	int area = r2 << 2;
	int rr = r << 1;

	for (int i = 0; i < area; i++)
	{
		int tx = (i % rr) - r;
		int ty = (i / rr) - r;

		if (tx * tx + ty * ty <= r2)
			DrawPoint(renderer, x + tx, y + ty);
	}
}

void drawDomino(SDL_Renderer *renderer, int n, int x, int y, int r, int g)
{
	for (int i = 0; i < 4; i++)
	{
		if (b[n][i] == 0)
		{
			DrawCircle(renderer, dx[i] * g + x, dy[i] * g + y, r);
		}
		else
		{
			FillCircle(renderer, dx[i] * g + x, dy[i] * g + y, r);
		}
	}
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

	cx = sw / 2;
	cy = sh / 2;

	r = 64;
	g = r * 2;

	while (1)
	{
		SDL_SetRenderTarget(renderer, display);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

		SDL_RenderClear(renderer);


		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 80);
		SDL_RenderDrawRect(renderer, &(SDL_Rect){ox, oy, sw, sh});

		time_t timestamp = time(NULL);
		struct tm *tm = localtime(&timestamp);

		int h = tm->tm_hour % 12;
		int m = tm->tm_min / 5;

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		drawDomino(renderer, h, cx + ox, cy + oy - g * 2, r, g);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		drawDomino(renderer, m, cx + ox, cy + oy + g * 2, r, g);

		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, display, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(display);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
