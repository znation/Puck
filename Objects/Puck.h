#include "Box2D\Box2D.h"
#include "Utility.h"

#pragma once

class Puck
{
public:
	Puck(b2Vec2 viewportSize, DeviceContext *ctx, b2World *world);
	void applyConstraints();
	void draw();
	void move();
	void reset();

private:
	b2World *m_world;
	b2Vec2 m_viewportSize;
	void adjustSpeed(double factor);
	D2D1_ELLIPSE m_ellipse;
	DeviceContext *m_ctx;
	SolidColorBrush *m_brush;
	b2Vec2 m_position;
	float m_radius;
	float m_maxSpeed;
	b2Body *m_circleBody;
};
