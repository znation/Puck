#include "pch.h"
#include "Powerup.h"

Powerup::Powerup(b2Vec2 viewportSize, b2Vec2 position, DeviceContext *ctx)
	:	m_viewportSize(viewportSize),
		m_position(position),
		m_ctx(ctx)
{
}
