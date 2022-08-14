#include <SDL2/SDL.h>

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

int sz;
int sw, sh;
int w, h;
int x, y, r;
int ox, oy;
int dx, dy;

SDL_Color c;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *display;

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

void init(void)
{
	r = 64;
	x = rand() % (sw - r * 2) + r;
	y = r;

	c.r = rand() % 256;
	c.g = rand() % 256;
	c.b = rand() % 256;
	c.a = 255;

	dx = (rand() % 2 ? -1 : 1) * (rand() % 10 + 5);
	dy = (rand() % 2 ? -1 : 1) * (rand() % 10 + 5);
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

	SDL_SetRenderTarget(renderer, display);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	SDL_RenderClear(renderer);

	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, display, NULL, NULL);
	SDL_RenderPresent(renderer);

	ox = (w - sw) / 2;
	oy = (h - sh) / 2;

	init();

	while (1)
	{
		SDL_SetRenderTarget(renderer, display);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 20);
		SDL_RenderFillRect(renderer, &(SDL_Rect){0, 0, w, h});

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, &(SDL_Rect){ox, oy, sw, sh});

		SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
		DrawCircle(renderer, x + ox, y + oy, r);

		dy++;

		x += dx;
		y += dy;

		if (x + dx < r)
		{
			x = r;
			dx = abs(dx);
		}
		if (y + dy < r)
		{
			y = r;
			dy = abs(dy);
		}
		if (x + dx >= sw - r)
		{
			x = sw - r;
			dx = -abs(dx);
		}
		if (y + dy >= sh - r)
		{
			y = sh - r;
			dy = -abs(dy);
		}

		if (y == sh - r && dy == 0)
		{
			init();
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
