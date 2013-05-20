#include "Utility.h"

#pragma once

class Powerup
{
public:
	Powerup(b2Vec2 playerSize, b2Vec2 playerPosition, b2World *world, b2Body *groundBody);
	void draw(DeviceContext *ctx, SolidColorBrush *brush);

private:
	float m_radius;
	b2Vec2 m_position;
	b2Body *m_circleBody;
	D2D1_ELLIPSE m_ellipse;
};