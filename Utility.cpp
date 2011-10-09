#include "Utility.h"

double scalingFactor = 200.0;
double frameRate = 60.0;

int boxToPixel(double coord)
{
	return (int)(coord * scalingFactor);
}

double pixelToBox(int coord)
{
	return ((double)coord) / scalingFactor;
}

bool rectContainsPoint(D2D1_RECT_F r, b2Vec2 p)
{
	return (p.x >= r.left &&
		p.x <= r.right &&
		p.y >= r.top &&
		p.y <= r.bottom);
}
