#include "pch.h"
#include "Player.h"
#include "Utility.h"
#include "Scene.h"

Player::Player(b2Vec2 sceneSize,
		b2Vec2 scenePosition,
		DeviceContext *ctx,
		int playerIdx,
		b2World *world,
		b2Body *groundBody,
		WriteFactory *dwriteFactory,
		Scene *scene)
{
	m_scene = scene;
	m_ctx = ctx;
	m_winner = false;
	float padding = 0.02f * sceneSize.y;
	m_playerIdx = playerIdx;
	m_size = b2Vec2((sceneSize.x / 2.0f) - (padding * 1.5f), sceneSize.y - (padding * 2.0f));
	m_position = b2Vec2(playerIdx == 0 ? scenePosition.x + padding : (sceneSize.x/2.0f) + scenePosition.x + (0.5f * padding),
            scenePosition.y + padding);
	m_stick = new Stick(m_size, m_position, world, groundBody);	
	m_goal = new Goal(sceneSize, scenePosition, m_playerIdx, ctx, world, scene);
	
	ThrowIfFailed(ctx->CreateSolidColorBrush(
		D2D1::ColorF(m_playerIdx == 0 ? D2D1::ColorF::Yellow : D2D1::ColorF::Magenta),
        &m_brush));

	ThrowIfFailed(
		dwriteFactory->CreateTextFormat(
		L"Segoe UI",
		nullptr,
		DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		(0.08f * sceneSize.y),
		L"en-US",
		&m_format));

	ThrowIfFailed(
		m_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER)
		);

	ThrowIfFailed(
		m_format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));

	m_rect.rect.top = m_position.y;
	m_rect.rect.left = m_position.x;
	m_rect.rect.bottom = m_position.y + m_size.y;
	m_rect.rect.right = m_position.x + m_size.x;
	m_rect.radiusX = 0.0125f * sceneSize.y;
	m_rect.radiusY = 0.0125f * sceneSize.y;
	m_brushStrokeSize = 0.0025f * sceneSize.y;
}

Player::~Player()
{
	delete m_goal;
	delete m_stick;
}

void Player::reset()
{
	m_stick->reset();
}

bool Player::containsPoint(b2Vec2 p)
{
	return rectContainsPoint(m_rect.rect, p);
}

void Player::draw()
{
	m_ctx->DrawRoundedRectangle(&m_rect,
		*(&m_brush),
		m_brushStrokeSize);

	m_goal->draw();

	m_stick->draw(m_ctx, m_brush);

	if (m_winner)
	{
		m_ctx->DrawText(
		L"WINNER",
		6,
		m_format,
		&(m_rect.rect),
		m_brush);
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
