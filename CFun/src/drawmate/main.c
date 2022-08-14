#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "sweetie16.h"

int fps = 2;

int sc;
int sw, sh;
int w, h;
int x, y, r;
int ox, oy;
int dx, dy;
int ps;

int scx, scy;
int svh, svv;
int smh, smv;
int sbs;
int sbx,sby;

int gx, gy;

int fid = -1;

bool holdh = false, holdv = false;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *display;
SDL_Event event;

TTF_Font *font;
SDL_Surface *txtsrf;
SDL_Texture *txttex;

char txt[256];

SDL_Point touch;
bool touchDown = false;

int bitmap_width = 16;
int bitmap_height = 16;
int bitmap_frames = 4;
int *bitmap_pixels = NULL;

bool quit = false;

int sgn(int x)
{
	return x < 0 ? -1 : x > 0 ? 1 : 0;
}

bool inrect(int x, int y, int rx, int ry, int rw, int rh)
{
	return x >= rx && x <= rx + rw && y >= ry && y <= ry + rh;
}

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

void drawScrollBar(SDL_Renderer *renderer, int sx, int sy, int sw, int sh, int *sv, int sm, int ish)
{
	int diffx = 0, diffy = 0;

	if (ish == 1)
	{
		SDL_RenderDrawRect(renderer, &(SDL_Rect){sx + ox, sy + oy, sw, sh});
		SDL_RenderDrawRect(renderer, &(SDL_Rect){sbx + ox, sy + oy, sh, sh});
	}
	else
	{
		SDL_RenderDrawRect(renderer, &(SDL_Rect){sx + ox, sy + oy, sw, sh});
		SDL_RenderDrawRect(renderer, &(SDL_Rect){sx + ox, *sv + oy, sw, sw});
	}

	int numTouchDevices = SDL_GetNumTouchDevices();
	for (int j = 0; j < numTouchDevices; j++)
	{
		SDL_TouchID touchId = SDL_GetTouchDevice(j);

		int numTouchFingers = SDL_GetNumTouchFingers(touchId);

		for (int i = 0; i < numTouchFingers; i++)
		{
			SDL_Finger *finger = SDL_GetTouchFinger(touchId, i);

			if (finger)
			{
				int tx = finger->x * w;
				int ty = finger->y * h;

				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				DrawCircle(renderer, tx, ty, 32);

				if (ish == 1)
				{
					if (!holdh)
					{
						if (inrect(tx, ty, sx + ox, sy + oy, sw, sh))
						{
							if (!holdh)
							{
								holdh = true;
							}
						}
					}
					else
					{
						sbx = tx - ox;
						if (sbx < 0)
							sbx = 0;
						if (sbx >= sw - sbs)
							sbx = sw - sbs;
							
						*sv=(double)sbx/(sw-sbs)*sm;
					}
				}
			}
		}
	}
}

void drawGrid(SDL_Renderer *renderer, int gx, int gy, int frame, int ps)
{
	int k = frame;

	for (int j = 0; j < bitmap_height; j++)
	{
		for (int i = 0; i < bitmap_width; i++)
		{
			int p = bitmap_pixels[j * bitmap_width + i + k * bitmap_width * bitmap_height];

			if (p != -1)
			{
				int r = sweetie16[p][0];
				int g = sweetie16[p][1];
				int b = sweetie16[p][2];
				int a = 255;

				SDL_SetRenderDrawColor(renderer, r, g, b, a);
				SDL_RenderFillRect(renderer, &(SDL_Rect){i * ps * sc + ox + gx, j * ps * sc + oy + gy, ps * sc, ps * sc});
			}

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(renderer, &(SDL_Rect){i * ps * sc + ox + gx, j * ps * sc + oy + gy, ps * sc, ps * sc});
		}
	}
}

int main(int argc, char *args[])
{
	srand(time(NULL));

	sc = 2;
	ps = 8;
	sbs = 64;

	sw = 360 * sc;
	sh = 646 * sc;
	w = sw;
	h = sh;

	bitmap_pixels = malloc(bitmap_width * bitmap_height * bitmap_frames * sizeof(*bitmap_pixels));

	memset(bitmap_pixels, -1, bitmap_width * bitmap_height * bitmap_frames);

	SDL_Init(SDL_INIT_VIDEO);

	TTF_Init();

	font = TTF_OpenFont("lazy.ttf", 28 * sc);

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

	smh = 255;
	smv = 0;

	svh = 0;
	svv = 0;

	gx = 0;
	gy = 0;

	while (!quit)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
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

				if (event.tfinger.fingerId == fid)
				{
					holdh = false;
					holdv = false;
				}

				touchDown = false;
			}
		}

		SDL_SetRenderTarget(renderer, display);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		//		drawGrid(renderer, gx - svh, gy - svv, 0, 16);

		//		drawScrollBar(renderer, sw - sbs, 0, sbs, sh - sbs, &svv, smh, 0);

		sprintf(txt, "%04d", svh);

		txtsrf = TTF_RenderText_Solid(font, txt, (SDL_Color){sweetie16[4][0], sweetie16[4][1], sweetie16[4][2], sweetie16[4][3]});

		txttex = SDL_CreateTextureFromSurface(renderer, txtsrf);

		SDL_RenderCopyEx(renderer, txttex, NULL, &(SDL_Rect){ox, oy, txtsrf->w, txtsrf->h}, 0, &(SDL_Point){0, 0}, SDL_FLIP_NONE);

		SDL_FreeSurface(txtsrf);
		SDL_DestroyTexture(txttex);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		drawScrollBar(renderer, 0, sh - sbs, sw - sbs, sbs, &svh, smh, 1);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, &(SDL_Rect){ox, oy, sw, sh});

		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, display, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(display);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
