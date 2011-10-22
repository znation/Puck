#include "Score.h"
#include "Scene.h"
#include "Utility.h"

Score::Score(b2Vec2 scoreSize,
		b2Vec2 scorePosition,
		int playerIdx,
		ComPtr<ID2D1RenderTarget> ctx)
{
	m_score = 0;
	m_ctx = ctx;
	m_playerIdx = playerIdx;

	b2Vec2 buttonSize = b2Vec2(scoreSize.y, scoreSize.y);
	for (int i=0; i<MAX_SCORE; i++)
	{
		b2Vec2 buttonPosition = b2Vec2(playerIdx == 0 ?
			scorePosition.x + (i * buttonSize.x) :
		(scorePosition.x + scoreSize.x) - ((i + 1) * buttonSize.x),
			scorePosition.y);
		m_scoreButtons[i] = new ScoreButton(ctx, buttonSize, buttonPosition);
	}
}

Score::~Score()
{
	for (int i=0; i<MAX_SCORE; i++)
	{
		delete m_scoreButtons[i];
	}
}

int Score::getScore()
{
	return m_score;
}

void Score::increment()
{
	m_scoreButtons[m_score]->SetFilled(true);
	m_score++;
}

void Score::draw()
{
	for (int i=0; i<MAX_SCORE; i++)
	{
		m_scoreButtons[i]->Draw();
	}
}

ScoreButton::ScoreButton(ComPtr<ID2D1RenderTarget> ctx,
						 b2Vec2 buttonSize,
						 b2Vec2 buttonPosition)
{
	m_ctx = ctx;
	m_size = buttonSize;
	m_position = buttonPosition;
	m_filled = false;

	m_ellipse.point.x = m_position.x + (m_size.x / 2.0f);
	m_ellipse.point.y = m_position.y + (m_size.y / 2.0f);
	m_ellipse.radiusX = (m_size.x / 2.0f) - 3.0f;
	m_ellipse.radiusY = (m_size.y / 2.0f) - 3.0f;
}

void ScoreButton::SetFilled(bool filled)
{
	m_filled = filled;
}

void ScoreButton::Draw()
{
	if (m_filled)
	{
		m_ctx->FillEllipse(
			&m_ellipse,
			Scene::Cyan.Get());
	}
	else
	{
		m_ctx->DrawEllipse(
			&m_ellipse,
			Scene::Cyan.Get());
	}
}
