#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "gameboy_palette.h"
#include "icon.h"

#define GRAPHICS_IMPLEMENTATION_H
#include "graphics.h"

#define UTIL_IMPLEMENTATION_H
#include "util.h"

#define BUTTON_IMPLEMENTATION_H
#include "button.h"

int tx, ty;
int fw, fh;
int x, y, z;
int i, j, k;
int w, h;
int sw = 160, sh = 144;
int cw = 32, ch = 32, cs = 8;
int ox = 0, oy = 0;
int *px = NULL;
int pc;

static SDL_Texture *display;
static SDL_Event event;
SDL_Point touch;
bool touchDown = false;
bool hold = false;
int currentColorIndex = 1;
int currentToolIndex = 0;
int prevToolIndex = 0;
bool grid = true;
int zoom = 8;
bool change = false;

void DrawTarget(int zoom)
{
	int x1, y1, x2, y2;

	SetDrawColor(renderer, gameboy_palette[5]);

	x1 = tx * zoom + ox + 1, y1 = ty * zoom + oy + 1;
	DrawRect(renderer, x1, y1, zoom, zoom);

	SetDrawColor(renderer, gameboy_palette[1]);

	x1 = tx * zoom + zoom + ox - 2, y1 = ty * zoom + oy + 3, x2 = tx * zoom + ox + 3, y2 = ty * zoom + zoom + oy - 2;

	DrawLine(renderer, x1, y1, x2, y2);

	x1 = tx * zoom + ox + 3, y1 = ty * zoom + oy + 3, x2 = tx * zoom + zoom + ox - 2, y2 = ty * zoom + zoom + oy - 2;

	DrawLine(renderer, x1, y1, x2, y2);
}

void EraseTarget(int zm, int c)
{
	z = ty * cw + tx;

	if (px[z] == 0)
	{
		SetDrawColor(renderer, gameboy_palette[5]);
	}
	else
	{
		SetDrawColor(renderer, gameboy_palette[px[z]]);
	}

	FillRect(renderer, tx * zoom + ox + 1, ty * zoom + oy + 1, zoom, zoom);

	if (grid)
	{
		SetDrawColor(renderer, gameboy_palette[1]);
		DrawRect(renderer, tx * zoom + ox + 1, ty * zoom + oy + 1, zoom, zoom);
	}
}

void DrawGrid(int zoom)
{
	int x1, y1, x2, y2;

	for (j = 0; j < ch; j++)
	{
		int y = j * zoom + oy + 1;

		if (y > sh + oy)
		{
			break;
		}

		for (i = 0; i < cw; i++)
		{
			int x = i * zoom + ox + 1;

			if (x > sw + ox)
			{
				break;
			}

			k = j * cw + i;
			if (px[k] == 0)
			{
				SetDrawColor(renderer, gameboy_palette[5]);
			}
			else
			{
				SetDrawColor(renderer, gameboy_palette[px[k]]);
			}

			FillRect(renderer, x, y, zoom, zoom);

			if (grid && zoom >= 4)
			{
				SetDrawColor(renderer, gameboy_palette[1]);
				DrawRect(renderer, x, y, zoom, zoom);
			}
		}
	}

	x1 = ox, y1 = oy, x2 = cw * zoom + ox, y2 = ch * zoom + oy;
	x2 = x2 > sw + ox ? sw + ox : x2;
	y2 = y2 > sh + oy ? sh + oy : y2;

	SetDrawColor(renderer, gameboy_palette[1]);
	DrawLine(renderer, x1, y1, x1, y2);
	DrawLine(renderer, x2, y1, x2, y2);
	DrawLine(renderer, x1, y1, x2, y1);
	DrawLine(renderer, x1, y2, x2, y2);
}

void EraseGrid(int zm, int c)
{
	SetDrawColor(renderer, gameboy_palette[c]);
	FillRect(renderer, ox, oy, cw * zm + 2, ch * zm + 2);
}

void DrawPalette(void)
{
	for (i = 0; i < gameboy_palette_length; i++)
	{
		if (i == 0)
		{
			SetDrawColor(renderer, gameboy_palette[5]);
		}
		else
		{
			SetDrawColor(renderer, gameboy_palette[i]);
		}
		FillRect(renderer, sw - 16 + ox, i * 16 + oy, 16, 16);

		if (i == currentColorIndex)
		{
			int x1, y1, x2, y2;

			x1 = sw - 16 + ox, y1 = i * 16 + oy, x2 = x1 + 14, y2 = y1 + 14;

			SetDrawColor(renderer, gameboy_palette[1]);
			DrawLine(renderer, x1, y1, x2, y2);
			SetDrawColor(renderer, gameboy_palette[5]);
			DrawLine(renderer, x1 + 1, y1, x2 + 1, y2);

			x1 = sw + ox - 2, y1 = i * 16 + oy, x2 = x1 - 15, y2 = y1 + 15;

			SetDrawColor(renderer, gameboy_palette[1]);
			DrawLine(renderer, x1, y1, x2, y2);
			SetDrawColor(renderer, gameboy_palette[5]);
			DrawLine(renderer, x1 + 1, y1, x2 + 1, y2);
		}

		SetDrawColor(renderer, gameboy_palette[1]);
		DrawRect(renderer, sw - 16 + ox, i * 16 + oy, 16, 16);
	}
}

void DrawImage(SDL_Renderer *renderer, int *px, int f, int w, int h, int x, int y, int sz, SDL_Color *pl)
{
	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			int k = j * w + i + f * w * h;
			SDL_Color c = pl[px[k]];
			SetDrawColor(renderer, c);
			FillRect(renderer, i * sz + x, j * sz + y, sz, sz);
		}
	}
}

void DrawTool(void)
{
	for (i = 0; i < icon_frames; i++)
	{
		DrawImage(renderer, icon_pixels, i, 8, 8, sw - 16 * 2 + ox, i * 8 * 2 + oy, 2, gameboy_palette);
	}

	SetDrawColor(renderer, gameboy_palette[5]);
	DrawRect(renderer, sw - 32 + ox, currentToolIndex * 16 + oy, 16, 16);
}

void FloodFill(int x, int y, int a, int b)
{
	if (a == b)
		return;

	if (x >= 0 && x < cw && y >= 0 && y < ch)
	{
		int z = y * cw + x;
		int c = px[z];
		if (c == a)
		{
			px[z] = b;
			FloodFill(x, y - 1, a, b);
			FloodFill(x, y + 1, a, b);
			FloodFill(x - 1, y, a, b);
			FloodFill(x + 1, y, a, b);
		}
	}
}

void Refresh()
{
	ox = (w - sw * pixelSize) / pixelSize / 2;
	oy = (h - sh * pixelSize) / pixelSize / 2;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	SetDrawColor(renderer, gameboy_palette[5]);
	FillRect(renderer, ox, oy, sw, sh);

	DrawGrid(zoom);
	DrawTarget(zoom);
	DrawPalette();
	DrawTool();
}

int main(int argc, char *args[])
{
	bool quit = false;

	w = sw;
	h = sh;

	tx = cw / 2;
	ty = ch / 2;

	px = calloc(cw * ch, sizeof(*px));

	for (i = 0; i < cw * ch; i++)
	{
		px[i] = 0;
	}

	srand(time(NULL));

	SDL_SetHint(SDL_HINT_ORIENTATIONS, "Portrait");

	SDL_Init(SDL_INIT_VIDEO);

	SDL_CreateWindowAndRenderer(w, h, 0, &window, &renderer);
	SDL_GetWindowSize(window, &w, &h);

	display = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

	SDL_SetRenderTarget(renderer, display);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	Refresh();

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
				/*
				
				if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{

					w = event.window.data1;
					h = event.window.data2;
					
					Refresh();

					SDL_RenderPresent(renderer);

				}
				*/
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

		if (touchDown)
		{
			if (!hold)
			{
				change = true;

				for (i = 0; i < gameboy_palette_length; i++)
				{
					if (inrect(touch.x / pixelSize, touch.y / pixelSize, sw - 16 + ox, i * 16 + oy, 16, 16))
					{
						hold = true;
						currentColorIndex = i;
						change = false;
						DrawPalette();
						break;
					}
				}

				for (i = 0; i < icon_frames; i++)
				{
					if (inrect(touch.x / pixelSize, touch.y / pixelSize, sw - 32 + ox, i * 16 + oy, 16, 16))
					{
						hold = true;
						prevToolIndex = currentToolIndex;
						currentToolIndex = i;
						change = false;
						DrawTool();
						break;
					}
				}
			}

			x = ((touch.x / pixelSize) - ox) / zoom;
			y = ((touch.y / pixelSize) - oy) / zoom;

			fw = sw / zoom;
			fw = fw < cw ? fw : cw;

			fh = sh / zoom;
			fh = fh < ch ? fh : ch;

			switch (currentToolIndex)
			{
			case 0:
				if (change)
				{
					if (x >= 0 && x < fw && y >= 0 && y < fh)
					{
						z = y * cw + x;

						if (!hold)
						{
							hold = true;

							pc = currentColorIndex;

							px[z] = pc;
						}
						else
						{
							px[z] = pc;
						}

						if (pc == 0)
						{
							SetDrawColor(renderer, gameboy_palette[5]);
						}
						else
						{
							SetDrawColor(renderer, gameboy_palette[px[z]]);
						}

						FillRect(renderer, x * zoom + ox +1, y * zoom + oy +1, zoom, zoom);
						
						if (grid)
						{
							SetDrawColor(renderer, gameboy_palette[1]);
							DrawRect(renderer, x * zoom + ox +1, y * zoom + oy +1, zoom, zoom);
						}
					}
				}
				break;

			case 1:
				zoom--;
				if (zoom < 1)
				{
					zoom = 1;
				}
				currentToolIndex = prevToolIndex;
				Refresh();
				break;
			case 2:
				zoom++;
				if (zoom > 8)
				{
					zoom = 8;
				}
				currentToolIndex = prevToolIndex;
				Refresh();
				break;
			case 3:
				grid = !grid;
				currentToolIndex = prevToolIndex;
				Refresh();
				break;
			case 4:
				if (change)
				{
					if (x >= 0 && x < fw && y >= 0 && y < fh)
					{
						tx = x;
						ty = y;
						Refresh();
					}
				}
				break;
			case 5:
				FloodFill(tx, ty, px[ty * cw + tx], currentColorIndex);
				Refresh();
				break;
			case 6:
				break;
			case 7:
				break;
			case 8:
				break;
			case 9:
				for (i = 0; i < cw * ch; i++)
				{
					px[i] = 0;
				}
				Refresh();
				break;
			}
		}
		else
		{
			hold = false;
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
