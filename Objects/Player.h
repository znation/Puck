#include <wrl.h>
#include <d2d1_1.h>
#include "Box2D\Box2D.h"
#include "Stick.h"
#include "Goal.h"
#include "Score.h"

#pragma once

using namespace Microsoft::WRL;

class Player
{
public:
	Player(b2Vec2 sceneSize,
		b2Vec2 scenePosition,
		ComPtr<ID2D1DeviceContext> ctx,
		int playerIdx,
		b2World *world,
		b2Body *groundBody,
		ComPtr<IDWriteFactory1> dwriteFactory);
	void draw(ComPtr<ID2D1DeviceContext> ctx);
	Stick *m_stick;
	bool containsPoint(b2Vec2 p);
	void detectCollisions();

private:
	int m_playerIdx;
	b2Vec2 m_size;
	b2Vec2 m_position;
	ComPtr<ID2D1SolidColorBrush> m_brush;
	D2D1_ROUNDED_RECT m_rect;
	Goal *m_goal;
	Score *m_score;
};
