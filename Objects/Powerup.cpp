#include "pch.h"
#include "Powerup.h"

Powerup::Powerup(b2Vec2 playerSize, b2Vec2 playerPosition, b2World *world, b2Body *groundBody)
	:	m_position(playerPosition)
{
	m_radius = 0.04f * playerSize.y;
	m_position = b2Vec2(playerPosition.x + (playerSize.x/2.0f),
		playerPosition.y + (playerSize.y/2.0f));
}
