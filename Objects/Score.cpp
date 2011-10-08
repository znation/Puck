#include "Score.h"
#include "DirectXSample.h"

Score::Score(b2Vec2 playerSize, b2Vec2 playerPosition, int playerIdx, ComPtr<ID2D1DeviceContext> ctx, ComPtr<IDWriteFactory1> dwriteFactory)
{
	m_ctx = ctx;
	m_playerIdx = playerIdx;

	int width = 200;
	int height = 300;
	int padding = 32;

	m_layout.top = playerPosition.y + padding;
	m_layout.left = playerIdx == 0 ?
		(playerSize.x + playerPosition.x - (padding + width)) :
	playerPosition.x + padding;
	m_layout.bottom = m_layout.top + height;
	m_layout.right = m_layout.left + width;

	DX::ThrowIfFailed(
		dwriteFactory->CreateTextFormat(
		L"Segoe UI",
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		64.0f,
		L"en-US",
		&m_format));

	DX::ThrowIfFailed(
		m_format->SetTextAlignment(m_playerIdx == 0 ? DWRITE_TEXT_ALIGNMENT_TRAILING : DWRITE_TEXT_ALIGNMENT_LEADING)
		);

	DX::ThrowIfFailed(ctx->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::GhostWhite),
		&m_brush));

	setScore(0);
}

void Score::setScore(int score)
{
	m_score = score;

	char buf[32];
	sprintf(buf, "%d", m_score);
	mbstowcs_s(&m_scoreTextLength, m_scoreText, strlen(buf)+1, buf, _TRUNCATE);
}

int Score::getScore()
{
	return m_score;
}

void Score::increment()
{
	setScore(m_score+1);
}

void Score::draw()
{
	m_ctx->DrawText(
		m_scoreText,
		m_scoreTextLength,
		m_format.Get(),
		&m_layout,
		m_brush.Get());
}
