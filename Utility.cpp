#include "pch.h"
#include "Utility.h"

float scalingFactor = 200.0f;

float32 boxToPixel(float32 coord)
{
	return coord * scalingFactor;
}

float32 pixelToBox(float32 coord)
{
	return coord / scalingFactor;
}

bool rectContainsPoint(D2D1_RECT_F r, b2Vec2 p)
{
	return (p.x >= r.left &&
		p.x <= r.right &&
		p.y >= r.top &&
		p.y <= r.bottom);
}

void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		abort();
	}
}

IUnknown* GetIUnknown(Windows::UI::Core::CoreWindow^ window)
{
	return (IUnknown*) reinterpret_cast<IUnknown*>(window);
}

