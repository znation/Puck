#include "Box2D\Box2D.h"
#include <d2d1_1helper.h>

#pragma once

#ifdef DEBUG
#define MAX_SCORE 2
#else
#define MAX_SCORE 11
#endif

#define MENU_BUTTON_COUNT 2
#define FRAMERATE 60.0f

float32 pixelToBox(float32 coord);
float32 boxToPixel(float32 coord);
bool rectContainsPoint(D2D1_RECT_F rect, b2Vec2 point);
