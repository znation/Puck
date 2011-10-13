#include "Box2D\Box2D.h"
#include <d2d1_1helper.h>

#pragma once

#ifdef DEBUG
#define MAX_SCORE 2
#else
#define MAX_SCORE 11
#endif

double pixelToBox(int coord);
int boxToPixel(double coord);
bool rectContainsPoint(D2D1_RECT_F rect, b2Vec2 point);
