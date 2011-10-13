#include <wrl.h>
#include <d2d1_1.h>
#include "Box2D\Box2D.h"
#include <dwrite_1.h>
#include "Utility.h"

#pragma once

using namespace Microsoft::WRL;

class ScoreButton
{
public:
	ScoreButton(ComPtr<ID2D1DeviceContext> ctx,
						 b2Vec2 buttonSize,
						 b2Vec2 buttonPosition,
						 ComPtr<ID2D1SolidColorBrush> brush);
	void SetFilled(bool filled);
	void Draw();
private:
	ComPtr<ID2D1DeviceContext> m_ctx;
	b2Vec2 m_size;
	b2Vec2 m_position;
	ComPtr<ID2D1SolidColorBrush> m_brush;
	bool m_filled;
	D2D1_ELLIPSE m_ellipse;
};

class Score
{
public:
	Score(b2Vec2 scoreSize,
		b2Vec2 scorePosition,
		int playerIdx,
		ComPtr<ID2D1DeviceContext> ctx);
	void draw();
	void increment();
	int getScore();
	void Reset();

private:
	int m_playerIdx;
	ComPtr<ID2D1DeviceContext> m_ctx;
	int m_score;
	ComPtr<ID2D1SolidColorBrush> m_brush;
	ScoreButton *m_scoreButtons[MAX_SCORE];
};
