#include "Box2D\Box2D.h"
#include <dwrite_1.h>
#include "Utility.h"

#pragma once

class ScoreButton
{
public:
	ScoreButton(ComPtr<ID2D1RenderTarget> ctx,
						 b2Vec2 buttonSize,
						 b2Vec2 buttonPosition);
	void SetFilled(bool filled);
	void Draw();
private:
	ComPtr<ID2D1RenderTarget> m_ctx;
	b2Vec2 m_size;
	b2Vec2 m_position;
	bool m_filled;
	D2D1_ELLIPSE m_ellipse;
};

class Score
{
public:
	Score(b2Vec2 scoreSize,
		b2Vec2 scorePosition,
		int playerIdx,
		ComPtr<ID2D1RenderTarget> ctx);
	~Score();
	void draw();
	void increment();
	int getScore();
	void Reset();

private:
	int m_playerIdx;
	ComPtr<ID2D1RenderTarget> m_ctx;
	int m_score;
	ScoreButton *m_scoreButtons[MAX_SCORE];
};
