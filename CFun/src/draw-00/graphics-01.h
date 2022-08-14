#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>

extern int pixelSize;

void DrawPoint(SDL_Renderer *renderer, int x, int y);

void DrawLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2);

void DrawThickLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int size);

void DrawRect(SDL_Renderer *renderer, int x, int y, int w, int h);

void DrawRect(SDL_Renderer *renderer, int x, int y, int w, int h);

void FillCircle(SDL_Renderer *renderer, int x, int y, int r);

void DrawLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2);

void DrawRect(SDL_Renderer *renderer, int x, int y, int w, int h);

void FillRect(SDL_Renderer *renderer, int x, int y, int w, int h);

void DrawCircle(SDL_Renderer *renderer, int x0, int y0, int r);

#ifdef GRAPHICS_IMPLEMENTATION_H

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *display;
static SDL_Event event;

int pixelSize = 4;

double sgn(double x)
{
	return x < 0 ? -1 : x > 0 ? 1 : 0;
}

bool inrect(int x, int y, int rx, int ry, int rw, int rh)
{
	return x >= rx && x < rx + rw && y >= ry && y < ry + rh;
}

bool incirc(int x, int y, int cx, int cy, int cr)
{
	return (cx - x) * (cx - x) + (cy - y) * (cy - y) <= cr * cr;
}

void DrawPoint(SDL_Renderer *renderer, int x, int y)
{
	SDL_RenderFillRect(renderer, &(SDL_Rect){x*pixelSize,y*pixelSize, pixelSize,pixelSize});
}

void DrawLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2)
{
	int i, dx, dy, sdx, sdy, dxabs, dyabs, x, y, px, py;

	dx = x2 - x1; /* the horizontal distance of the line */
	dy = y2 - y1; /* the vertical distance of the line */
	dxabs = abs(dx);
	dyabs = abs(dy);
	sdx = sgn(dx);
	sdy = sgn(dy);
	x = dyabs >> 1;
	y = dxabs >> 1;
	px = x1;
	py = y1;

	DrawPoint(renderer, px, py);

	if (dxabs >= dyabs)
	{ /* the line is more horizontal than vertical */
		for (i = 0; i < dxabs; i++)
		{
			y += dyabs;
			if (y >= dxabs)
			{
				y -= dxabs;
				py += sdy;
			}
			px += sdx;
			DrawPoint(renderer, px, py);
		}
	}
	else
	{ /* the line is more vertical than horizontal */
		for (i = 0; i < dyabs; i++)
		{
			x += dxabs;
			if (x >= dyabs)
			{
				x -= dyabs;
				px += sdx;
			}
			py += sdy;
			DrawPoint(renderer, px, py);
		}
	}
}

void DrawThickLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int size)
{
	int i, dx, dy, sdx, sdy, dxabs, dyabs, x, y, px, py;

	dx = x2 - x1; /* the horizontal distance of the line */
	dy = y2 - y1; /* the vertical distance of the line */
	dxabs = abs(dx);
	dyabs = abs(dy);
	sdx = sgn(dx);
	sdy = sgn(dy);
	x = dyabs >> 1;
	y = dxabs >> 1;
	px = x1;
	py = y1;

	FillCircle(renderer, px, py, size);

	if (dxabs >= dyabs)
	{ /* the line is more horizontal than vertical */
		for (i = 0; i < dxabs; i++)
		{
			y += dyabs;
			if (y >= dxabs)
			{
				y -= dxabs;
				py += sdy;
			}
			px += sdx;
			FillCircle(renderer, px, py, size);
		}
	}
	else
	{ /* the line is more vertical than horizontal */
		for (i = 0; i < dyabs; i++)
		{
			x += dxabs;
			if (x >= dyabs)
			{
				x -= dyabs;
				px += sdx;
			}
			py += sdy;
			FillCircle(renderer, px, py, size);
		}
	}
}

void DrawRect(SDL_Renderer *renderer, int x, int y, int w, int h)
{
	int i, j;

	for (i = 0; i < w; i++)
	{
		DrawPoint(renderer, x + i, y);
		DrawPoint(renderer, x + i, y + h - 1);
	}

	for (j = 0; j < h; j++)
	{
		DrawPoint(renderer, x, y + j);
		DrawPoint(renderer, x + w - 1, y + j);
	}
}

void FillRect(SDL_Renderer *renderer, int x, int y, int w, int h)
{
	int i, j;
	for (j = 0; j < h; j++)
	{
		for (i = 0; i < w; i++)
		{
			DrawPoint(renderer, x + i, y + j);
		}
	}
}

void DrawCircle(SDL_Renderer *renderer, int x0, int y0, int r)
{
	int f = 1 - r;
	int ddF_x = 0;
	int ddF_y = -2 * r;
	int x = 0;
	int y = r;

	DrawPoint(renderer, x0, y0 + r);
	DrawPoint(renderer, x0, y0 - r);
	DrawPoint(renderer, x0 + r, y0);
	DrawPoint(renderer, x0 - r, y0);

	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;

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

void DrawPoly(SDL_Renderer *renderer, int n, int *v)
{
	int i;
	for (i = 0; i < n - 1; i++)
	{
		DrawLine(
			renderer,
			v[(i << 1) + 0],
			v[(i << 1) + 1],
			v[(i << 1) + 2],
			v[(i << 1) + 3]);
	}
	
	DrawLine(
		renderer,
		v[0],
		v[1],
		v[(n << 1) - 2],
		v[(n << 1) - 1]);
}

#endif

#endif
