#pragma once

using namespace Microsoft::WRL;

#define DeviceContext ID2D1DeviceContext
#define WriteFactory IDWriteFactory1
#define D2DFactory ID2D1Factory1
#define D2DBitmap ID2D1Bitmap1
#define ImagingFactory IWICImagingFactory2
#define D3DDevice ID3D11Device1

class PointProperties
{
public:
	bool IsLeftButtonPressed;
	bool IsRightButtonPressed;
};

class Position
{
public:
	float X;
	float Y;
};

class Point
{
public:
	Position Position;
	PointProperties *Properties;
};

class PointerEventArgs
{
public:
	Point *CurrentPoint;

private:
	Windows::UI::Core::PointerEventArgs^ m_args;
};

#define SolidColorBrush ID2D1SolidColorBrush
