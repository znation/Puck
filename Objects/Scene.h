#include <wrl.h>
#include <d2d1_1.h>
#include "Box2D\Box2D.h"
#include "Player.h"
#include "Puck.h"
#include "TopBar.h"

#pragma once

using namespace Microsoft::WRL;

class Scene
{
public:
	Scene(b2Vec2 viewportSize, ComPtr<ID2D1DeviceContext> ctx, b2World *world, ComPtr<IDWriteFactory1> dwriteFactory);
	void move();
	void draw();
	void applyConstraints();
	void detectCollisions();
	void onMouseMoved(b2Vec2 p);
	void win(int playerIdx);
	void reset();
	void scoreGoal(int playerIdx);

private:
	void drawGrid();
	bool drawRoundTimer();
	void beginRound();

	ComPtr<ID2D1DeviceContext> m_ctx;
	b2Vec2 m_size;
	b2Vec2 m_position;
	ComPtr<ID2D1SolidColorBrush> m_brush;
	ComPtr<ID2D1SolidColorBrush> m_gridBrush;
	D2D1_ROUNDED_RECT m_rect;
	Player *m_players[2];
	Puck *m_puck;
	TopBar *m_topBar;
	D2D1_RECT_F m_groundBoxRect;
	b2Body *m_groundBoxBody;
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
