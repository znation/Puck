#include <wrl.h>
#include <d2d1_1.h>
#include "Box2D\Box2D.h"
#include "Stick.h"
#include "Goal.h"
#include "Score.h"
#include <dwrite_1.h>

#pragma once

using namespace Microsoft::WRL;

// forward declaration to prevent circular dependency
class Scene;

class Player
{
public:
	Player(b2Vec2 sceneSize,
		b2Vec2 scenePosition,
		ComPtr<ID2D1DeviceContext> ctx,
		int playerIdx,
		b2World *world,
		b2Body *groundBody,
		ComPtr<IDWriteFactory1> dwriteFactory,
		Scene *scene);
	void draw();
	Stick *m_stick;
	bool containsPoint(b2Vec2 p);
	void detectCollisions();
	void showWinnerText();
	void reset();

private:
	ComPtr<ID2D1DeviceContext> m_ctx;
	bool m_winner;
	int m_playerIdx;
	b2Vec2 m_size;
	b2Vec2 m_position;
	ComPtr<ID2D1SolidColorBrush> m_brush;
	ComPtr<IDWriteTextFormat> m_format;
	D2D1_ROUNDED_RECT m_rect;
	Goal *m_goal;
	Scene *m_scene;
};
