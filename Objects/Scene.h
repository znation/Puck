#include "Box2D\Box2D.h"
#include "Player.h"
#include "Puck.h"
#include "TopBar.h"
#include "b2UserData.h"
#include "Utility.h"

#pragma once

class Game;

class Scene
{
public:
	static SolidColorBrush *Cyan;

	Scene(b2Vec2 viewportSize,
			 DeviceContext *ctx,
			 b2World *world,
			 WriteFactory *dwriteFactory,
			 Game *game);
	~Scene();
	void move();
	void draw();
	void applyConstraints();
	void detectCollisions();
	void onMouseMoved(b2Vec2 p);
	void win(int playerIdx);
	void reset();
	void scoreGoal(int playerIdx);
	void OnMouseDown(b2Vec2 p);
	void Resize(b2Vec2 viewportSize);

private:
	class Edge
	{
	public:
		Edge();
		b2Vec2 center;
		b2Vec2 extents;
		const char *extraInfo;
		void Initialize(b2Vec2 center_, b2Vec2 extents_, const char *e);
	};

	void drawGrid();
	bool drawRoundTimer();
	void beginRound();
	void resetCore();

	DeviceContext *m_ctx;
	b2Vec2 m_size;
	b2Vec2 m_position;
	SolidColorBrush *m_gridBrush;
	D2D1_ROUNDED_RECT m_rect;
	Player *m_players[2];
	b2UserData *m_userData[4];
	Puck::Puck *m_puck;
	TopBar *m_topBar;
	bool m_gameOver;
	bool m_frozen;
	ULONGLONG m_beginTime;
	IDWriteTextFormat *m_roundTimerTextFormat;
	Edge m_edges[4];
	b2Body *m_groundBoxBody[4];
	b2World *m_world;
};
