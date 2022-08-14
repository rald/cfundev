#ifndef BUTTON_H
#define BUTTON_H

#define GRAPHICS_IMPLEMENTATION_H
#include "graphics.h"

#define UTIL_IMPLEMENTATION_H
#include "util.h"

typedef struct button_t button_t;

struct button_t
{
	int x, y, w, h;
	SDL_Color color;
	bool isDown;
};

button_t *CreateButton(int x, int y, int w, int h, SDL_Color color);
void DestroyButton(button_t **button);
void DrawButton(SDL_Renderer *renderer, button_t *button);
bool HandleButtonEvents(button_t *button, SDL_Point touch);

#ifdef BUTTON_IMPLEMENTATION_H

button_t *CreateButton(int x, int y, int w, int h, SDL_Color color)
{
	button_t *button = malloc(sizeof(*button));
	if (button)
	{
		button->x = x;
		button->y = y;
		button->w = w;
		button->h = h;
		button->color = color;
		button->isDown = false;
	}
	return button;
}

void DestroyButton(button_t **button)
{
	free(*button);
	button = NULL;
}

void DrawButton(SDL_Renderer *renderer, button_t *button)
{
	if (button->isDown)
	{
		SetDrawColor(renderer, palette[4]);
		DrawLine(renderer, button->x, button->y, button->x + button->w - 1, button->y + button->h - 1);
		DrawLine(renderer, button->x + button->w - 1, button->y, button->x, button->y + button->h - 1);
		DrawRect(renderer, button->x, button->y, button->w, button->h);
	}
	else
	{
		SetDrawColor(renderer, button->color);
		FillRect(renderer, button->x, button->y, button->w, button->h);
		SetDrawColor(renderer, palette[0]);
		DrawRect(renderer, button->x, button->y, button->w, button->h);
	}

}

bool HandleButtonEvents(button_t *button, SDL_Point touch)
{
	if (inrect(touch.x / pixelSize, touch.y / pixelSize, button->x, button->y, button->w, button->h))
	{
		button->isDown = true;
	}
	else
	{
		button->isDown = false;
	}

	return button->isDown;
}

#endif

#endif
