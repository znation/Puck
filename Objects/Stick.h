#include <wrl.h>
#include <d2d1_1.h>
#include "Box2D\Box2D.h"

#pragma once

using namespace Microsoft::WRL;

class Stick
{
public:
	Stick(b2Vec2 playerSize, b2Vec2 playerPosition, b2World *world, b2Body *groundBody);
	void updatePosition(int x, int y);
	void move();
	void draw(ComPtr<ID2D1DeviceContext> ctx, ComPtr<ID2D1SolidColorBrush> brush);
	void reset();

private:
	void updateEllipse();
	int m_radius;
	b2Vec2 m_position;
	b2Vec2 m_nextPosition;
	b2Vec2 m_startingPosition;
	int m_innerCircleWidth;
	int m_outerCircleWidth;
	D2D1_ELLIPSE m_innerEllipse;
	D2D1_ELLIPSE m_outerEllipse;
	b2MouseJoint *m_boxMouseJoint;
	b2Body *m_circleBody;
};
