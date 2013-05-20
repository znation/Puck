#include "Box2D\Box2D.h"
#include "b2UserData.h"
#include "Player.h"
#include "Puck.h"
#include "TopBar.h"
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
	std::vector<Player> m_players; // should have 2 inside
	std::vector<b2UserData> m_userData; // should have 4 inside
	b2Body * m_groundBoxBody[4]; // not a smart pointer because dtr is hidden
	std::unique_ptr<::Puck::Puck> m_puck;
	std::unique_ptr<TopBar> m_topBar;
	bool m_gameOver;
	bool m_frozen;
	ULONGLONG m_beginTime;
	IDWriteTextFormat *m_roundTimerTextFormat;
	Edge m_edges[4];
	b2World *m_world;
};
