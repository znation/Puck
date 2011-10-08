#include "Player.h"
#include "DirectXSample.h"

Player::Player(b2Vec2 sceneSize, b2Vec2 scenePosition, ComPtr<ID2D1DeviceContext> ctx, int playerIdx, b2World *world, b2Body *groundBody)
{
	int padding = 16;
	m_playerIdx = playerIdx;
	m_size = b2Vec2((sceneSize.x / 2.0) - (padding * 1.5), sceneSize.y - (padding * 2.0));
	m_position = b2Vec2(playerIdx == 0 ? scenePosition.x + padding : (sceneSize.x/2.0) + scenePosition.x + (0.5 * padding),
            scenePosition.y + padding);
	m_stick = new Stick(m_size, m_position, world, groundBody);
	m_goal = new Goal(sceneSize, scenePosition, m_playerIdx, ctx, world);
	
	DX::ThrowIfFailed(ctx->CreateSolidColorBrush(
		D2D1::ColorF(m_playerIdx == 0 ? D2D1::ColorF::Yellow : D2D1::ColorF::Magenta),
        &m_brush));

	m_rect.rect.top = m_position.y;
	m_rect.rect.left = m_position.x;
	m_rect.rect.bottom = m_position.y + m_size.y;
	m_rect.rect.right = m_position.x + m_size.x;
	m_rect.radiusX = 10;
	m_rect.radiusY = 10;
}

void Player::draw(ComPtr<ID2D1DeviceContext> ctx)
{
	ctx->DrawRoundedRectangle(&m_rect,
		*(&m_brush),
		2.0f);

	m_goal->draw(ctx);

	m_stick->draw(ctx, m_brush);
}

bool Player::containsPoint(b2Vec2 p)
{
	return (p.x >= m_rect.rect.left &&
		p.x <= m_rect.rect.right &&
		p.y >= m_rect.rect.top &&
		p.y <= m_rect.rect.bottom);
}

void Player::detectCollisions()
{
	m_goal->detectCollisions();
}
