#include "Utility.h"
#include <d2d1_1.h>
#include "Box2D\Box2D.h"
#include "Player.h"
#include "Puck.h"
#include "TopBar.h"
#include "b2UserData.h"

#pragma once

class Game;

class Scene
{
public:
	static ComPtr<ID2D1SolidColorBrush> Cyan;

	Scene(b2Vec2 viewportSize,
			 ComPtr<ID2D1DeviceContext> ctx,
			 b2World *world,
			 ComPtr<IDWriteFactory1> dwriteFactory,
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
#ifdef WINRT
	void OnMouseDown(Windows::UI::Core::PointerEventArgs^ args);
#endif

private:
	void drawGrid();
	bool drawRoundTimer();
	void beginRound();
	void resetCore();

	ComPtr<ID2D1DeviceContext> m_ctx;
	b2Vec2 m_size;
	b2Vec2 m_position;
	ComPtr<ID2D1SolidColorBrush> m_gridBrush;
	D2D1_ROUNDED_RECT m_rect;
	Player *m_players[2];
	b2UserData *m_userData[4];
	Puck *m_puck;
	TopBar *m_topBar;
	bool m_gameOver;
	bool m_frozen;
	ULONGLONG m_beginTime;
	ComPtr<IDWriteTextFormat> m_roundTimerTextFormat;
	
	class Edge
	{
	public:
		Edge(b2Vec2 center_, b2Vec2 extents_, const char *e);
		b2Vec2 center;
		b2Vec2 extents;
		const char *extraInfo;
	};
};
