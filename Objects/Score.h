#include "Box2D\Box2D.h"
#include "Utility.h"

#pragma once

class ScoreButton
{
public:
	ScoreButton(DeviceContext *ctx);
	void Resize(b2Vec2 buttonSize, b2Vec2 buttonPosition);
	void SetFilled(bool filled);
	void Draw();
private:
	DeviceContext *m_ctx;
	b2Vec2 m_size;
	b2Vec2 m_position;
	bool m_filled;
	D2D1_ELLIPSE m_ellipse;
};

class Score
{
public:
	Score(int playerIdx,
		DeviceContext *ctx);
	~Score();
	void Resize(b2Vec2 scoreSize,
		b2Vec2 scorePosition);
	void draw();
	void increment();
	int getScore();
	void Reset();

private:
	int m_playerIdx;
	DeviceContext *m_ctx;
	int m_score;
	ScoreButton *m_scoreButtons[MAX_SCORE];
};
