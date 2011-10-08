#include <wrl.h>
#include <d2d1_1.h>
#include "Box2D\Box2D.h"

#pragma once

using namespace Microsoft::WRL;

class Goal
{
public:
	Goal(b2Vec2 playerSize, b2Vec2 playerPosition, int playerIdx, ComPtr<ID2D1DeviceContext> ctx, b2World *world);
	void draw(ComPtr<ID2D1DeviceContext> ctx);
	void detectCollisions();

private:
	b2Vec2 m_size;
	b2Vec2 m_position;
	ComPtr<ID2D1SolidColorBrush> m_brush;
	D2D1_RECT_F m_rect;
	b2Body *m_goalBody;
};
