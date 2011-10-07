#include <wrl.h>
#include <d2d1_1.h>
#include "Box2D\Box2D.h"

#pragma once

using namespace Microsoft::WRL;

class Puck
{
public:
	Puck(b2Vec2 viewportSize, ComPtr<ID2D1DeviceContext> ctx, b2World *world);
	void applyConstraints();
	void detectCollisions();
	void draw(ComPtr<ID2D1DeviceContext> ctx);
	void move();
private:
	void adjustSpeed(double factor);
	D2D1_ELLIPSE m_ellipse;
	ComPtr<ID2D1SolidColorBrush> m_brush;
	b2Vec2 m_position;
	int m_radius;
	double m_maxSpeed;
	b2Body *m_circleBody;
};
