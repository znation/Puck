#ifdef WINRT
#include <wrl.h>
#endif
#include "Box2D\Box2D.h"
#include <d2d1_1helper.h>

#pragma once

#ifdef WINRT
using namespace Microsoft::WRL;
using namespace Windows::Storage;
using namespace Windows::Foundation;
using namespace Windows::Storage::Streams;
#else
#define ref 
#endif

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
IUnknown* GetIUnknown(Windows::UI::Core::CoreWindow^ window);
