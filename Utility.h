#include "Box2D\Box2D.h"

#ifdef WINRT
#include <wrl.h>
#include <d2d1_1.h>
#include <dwrite_1.h>
#include <d2d1_1helper.h>
#include <d3d11_1.h>
#include <dxgi1_2.h>
#include <d2d1effects.h>
#else
#include <d2d1helper.h>
#include <d2d1.h>
#include <dwrite.h>
#include <d3d11.h>
#include <dxgi.h>
#endif

#include "DirectXAbstrations.h"

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
void ThrowIfFailed(HRESULT hr);

#ifdef WINRT
IUnknown* GetIUnknown(Windows::UI::Core::CoreWindow^ window);
#endif
