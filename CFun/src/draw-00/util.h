#ifndef UTIL_H
#define UTIL_H

double sgn(double x);
bool inrect(int x, int y, int rx, int ry, int rw, int rh);
bool incirc(int x, int y, int cx, int cy, int cr);

#ifdef UTIL_IMPLEMENTATION_H

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

#endif

#endif