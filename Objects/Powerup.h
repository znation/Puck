#include "Utility.h"

#pragma once

class Powerup
{
public:
	Powerup(b2Vec2 viewportSize, b2Vec2 position, DeviceContext *ctx);
	void draw();

private:
	b2Vec2 m_viewportSize;
	b2Vec2 m_position;
	D2D1_ELLIPSE m_ellipse;
	DeviceContext *m_ctx;
	SolidColorBrush *m_brush;
};