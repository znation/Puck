#include "pch.h"
#include "Puck.h"
#include "Utility.h"
#include "b2UserData.h"

Puck::Puck::Puck(b2Vec2 viewportSize, DeviceContext *ctx, b2World *world)
{
	m_world = world;
	m_viewportSize = viewportSize;
	m_ctx = ctx;
	m_radius = 0.025f * viewportSize.y;
	m_maxSpeed = 3.0f;
	m_ellipse.radiusX = m_radius;
	m_ellipse.radiusY = m_radius;
	m_circleBody = nullptr;

	ThrowIfFailed(ctx->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Cyan),
		&m_brush));

	// Set initial position
	reset();
}

void Puck::Puck::reset()
{
	m_position = b2Vec2(m_viewportSize.x/2.0f, m_viewportSize.y/2.0f);
	m_ellipse.point.x = m_position.x;
	m_ellipse.point.y = m_position.y;
	
	if (m_circleBody != nullptr)
	{
		m_world->DestroyBody(m_circleBody);
		m_circleBody = nullptr;
	}

	b2BodyDef circleBd;
	circleBd.position.Set(pixelToBox(m_position.x), pixelToBox(m_position.y));
	circleBd.type = b2_dynamicBody;
	circleBd.fixedRotation = true; // TODO -- try setting this to false
	circleBd.bullet = true;
	m_circleBody = m_world->CreateBody(&circleBd);
	m_circleBody->SetUserData(new b2UserData(b2UserData_Puck, nullptr));
	b2CircleShape circleSd;
	circleSd.m_type = b2Shape::e_circle;
	circleSd.m_radius = pixelToBox(m_radius);
	b2FixtureDef circleF;
	circleF.shape = &circleSd;
	circleF.density = 1.0;
	circleF.friction = 0.0;
	circleF.restitution = 1.0;
	m_circleBody->CreateFixture(&circleF);
}

void Puck::Puck::draw()
{
	m_ctx->FillEllipse(&m_ellipse,
		*(&m_brush));
}

void Puck::Puck::move()
{
	b2Vec2 p = m_circleBody->GetPosition();
	m_position.Set(boxToPixel(p.x), boxToPixel(p.y));
	m_ellipse.point.x = m_position.x;
	m_ellipse.point.y = m_position.y;
}

void Puck::Puck::applyConstraints()
{
	b2Vec2 velocity = m_circleBody->GetLinearVelocity();
	float speed = velocity.Length();
	if (speed > m_maxSpeed)
	{
		velocity *= (m_maxSpeed / speed);
		m_circleBody->SetLinearVelocity(velocity);
	}
}

