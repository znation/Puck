#include "Player.h"
#include "DirectXSample.h"
#include "Utility.h"
#include "Scene.h"

Player::Player(b2Vec2 sceneSize,
		b2Vec2 scenePosition,
		ComPtr<ID2D1DeviceContext> ctx,
		int playerIdx,
		b2World *world,
		b2Body *groundBody,
		ComPtr<IDWriteFactory1> dwriteFactory,
		Scene *scene)
{
	m_scene = scene;
	m_ctx = ctx;
	m_winner = false;
	int padding = 16;
	m_playerIdx = playerIdx;
	m_size = b2Vec2((sceneSize.x / 2.0) - (padding * 1.5), sceneSize.y - (padding * 2.0));
	m_position = b2Vec2(playerIdx == 0 ? scenePosition.x + padding : (sceneSize.x/2.0) + scenePosition.x + (0.5 * padding),
            scenePosition.y + padding);
	m_stick = new Stick(m_size, m_position, world, groundBody);
	m_score = new Score(m_size, m_position, m_playerIdx, ctx, dwriteFactory);
	m_goal = new Goal(sceneSize, scenePosition, m_playerIdx, ctx, world, scene);
	
	DX::ThrowIfFailed(ctx->CreateSolidColorBrush(
		D2D1::ColorF(m_playerIdx == 0 ? D2D1::ColorF::Yellow : D2D1::ColorF::Magenta),
        &m_brush));

	DX::ThrowIfFailed(
		dwriteFactory->CreateTextFormat(
		L"Segoe UI",
		nullptr,
		DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		64.0f,
		L"en-US",
		&m_format));

	DX::ThrowIfFailed(
		m_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER)
		);

	DX::ThrowIfFailed(
		m_format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));

	m_rect.rect.top = m_position.y;
	m_rect.rect.left = m_position.x;
	m_rect.rect.bottom = m_position.y + m_size.y;
	m_rect.rect.right = m_position.x + m_size.x;
	m_rect.radiusX = 10;
	m_rect.radiusY = 10;
}

void Player::reset()
{
	m_stick->reset();
}

bool Player::containsPoint(b2Vec2 p)
{
	return rectContainsPoint(m_rect.rect, p);
}

void Player::scoreGoal()
{
	m_score->increment();
	int score = m_score->getScore();
	if (score == MAX_SCORE)
	{
		m_scene->win(m_playerIdx);
	}
	else
	{
		m_scene->reset();
	}
}

void Player::draw()
{
	m_ctx->DrawRoundedRectangle(&m_rect,
		*(&m_brush),
		2.0f);

	m_goal->draw();

	m_stick->draw(m_ctx, m_brush);

	m_score->draw();

	if (m_winner)
	{
		m_ctx->DrawText(
		L"WINNER",
		6,
		m_format.Get(),
		&(m_rect.rect),
		m_brush.Get());
	}
}

void Player::detectCollisions()
{
	m_goal->detectCollisions();
}

void Player::showWinnerText()
{
	m_winner = true;
}
