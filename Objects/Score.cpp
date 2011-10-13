#include "Score.h"
#include "DirectXSample.h"

Score::Score(b2Vec2 scoreSize,
		b2Vec2 scorePosition,
		int playerIdx,
		ComPtr<ID2D1DeviceContext> ctx)
{
	m_score = 0;
	m_ctx = ctx;
	m_playerIdx = playerIdx;

	DX::ThrowIfFailed(ctx->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::GhostWhite),
		&m_brush));

	b2Vec2 buttonSize = b2Vec2(scoreSize.y, scoreSize.y);
	for (int i=0; i<MAX_SCORE; i++)
	{
		b2Vec2 buttonPosition = b2Vec2(playerIdx == 0 ?
			scorePosition.x + (i * buttonSize.x) :
		(scorePosition.x + scoreSize.x) - ((i + 1) * buttonSize.x),
			scorePosition.y);
		m_scoreButtons[i] = new ScoreButton(ctx, buttonSize, buttonPosition, m_brush);
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

ScoreButton::ScoreButton(ComPtr<ID2D1DeviceContext> ctx,
						 b2Vec2 buttonSize,
						 b2Vec2 buttonPosition,
						 ComPtr<ID2D1SolidColorBrush> brush)
{
	m_ctx = ctx;
	m_size = buttonSize;
	m_position = buttonPosition;
	m_brush = brush;
	m_filled = false;

	m_ellipse.point.x = m_position.x + (m_size.x / 2.0);
	m_ellipse.point.y = m_position.y + (m_size.y / 2.0);
	m_ellipse.radiusX = (m_size.x / 2.0) - 1;
	m_ellipse.radiusY = (m_size.y / 2.0) - 1;
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
			m_brush.Get());
	}
	else
	{
		m_ctx->DrawEllipse(
			&m_ellipse,
			m_brush.Get());
	}
}
