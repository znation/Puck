#include "Utility.h"
#include "Box2D\Box2D.h"

#pragma once

class Stick
{
public:
	Stick(b2Vec2 playerSize, b2Vec2 playerPosition, b2World *world, b2Body *groundBody);
	void updatePosition(float x, float y);
	void move();
	void draw(ComPtr<ID2D1RenderTarget> ctx, ComPtr<ID2D1SolidColorBrush> brush);
	void reset();

private:
	void updateEllipse();
	float m_radius;
	b2Vec2 m_position;
	b2Vec2 m_nextPosition;
	b2Vec2 m_startingPosition;
	float m_innerCircleWidth;
	float m_outerCircleWidth;
	D2D1_ELLIPSE m_innerEllipse;
	D2D1_ELLIPSE m_outerEllipse;
	b2MouseJoint *m_boxMouseJoint;
	b2Body *m_circleBody;
};
