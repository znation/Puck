#include "Box2D\Box2D.h"
#include "Stick.h"
#include "Goal.h"
#include "Score.h"
#include "Utility.h"

#pragma once

// forward declaration to prevent circular dependency
class Scene;

class Player
{
public:
	Player(b2Vec2 sceneSize,
		b2Vec2 scenePosition,
		DeviceContext *ctx,
		int playerIdx,
		b2World *world,
		b2Body *groundBody,
		WriteFactory *dwriteFactory,
		Scene *scene);
	~Player();
	void draw();
	Stick *m_stick;
	bool containsPoint(b2Vec2 p);
	void detectCollisions();
	void showWinnerText();
	void reset();

private:
	DeviceContext *m_ctx;
	bool m_winner;
	int m_playerIdx;
	b2Vec2 m_size;
	b2Vec2 m_position;
	SolidColorBrush *m_brush;
	IDWriteTextFormat *m_format;
	D2D1_ROUNDED_RECT m_rect;
	Goal *m_goal;
	Scene *m_scene;
};
