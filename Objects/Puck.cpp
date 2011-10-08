#include "Puck.h"
#include "DirectXSample.h"
#include "D2DRenderer.h"
#include "b2UserData.h"

Puck::Puck(b2Vec2 viewportSize, ComPtr<ID2D1DeviceContext> ctx, b2World *world)
{
	m_radius = 20;
	m_position = b2Vec2(viewportSize.x/2.0, viewportSize.y/2.0);
	m_maxSpeed = 3.0;

	m_ellipse.point.x = m_position.x;
	m_ellipse.point.y = m_position.y;
	m_ellipse.radiusX = m_radius;
	m_ellipse.radiusY = m_radius;

	DX::ThrowIfFailed(ctx->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Cyan),
		&m_brush));

	b2BodyDef circleBd;
	circleBd.position.Set(pixelToBox(m_position.x), pixelToBox(m_position.y));
	circleBd.type = b2_dynamicBody;
	circleBd.fixedRotation = true; // TODO -- try setting this to false
	circleBd.bullet = true;
	m_circleBody = world->CreateBody(&circleBd);
	m_circleBody->SetUserData(new b2UserData(b2UserData_Puck, nullptr));
	b2CircleShape circleSd;
	circleSd.m_type = b2Shape::Type::e_circle;
	circleSd.m_radius = pixelToBox(m_radius);
	b2FixtureDef circleF;
	circleF.shape = &circleSd;
	circleF.density = 1.0;
	circleF.friction = 0.0;
	circleF.restitution = 1.0;
	m_circleBody->CreateFixture(&circleF);
}

void Puck::draw(ComPtr<ID2D1DeviceContext> ctx)
{
	ctx->FillEllipse(&m_ellipse,
		*(&m_brush));
}

void Puck::move()
{
	b2Vec2 p = m_circleBody->GetPosition();
	m_position.Set(boxToPixel(p.x), boxToPixel(p.y));
	m_ellipse.point.x = m_position.x;
	m_ellipse.point.y = m_position.y;
}

void Puck::applyConstraints()
{
	b2Vec2 velocity = m_circleBody->GetLinearVelocity();
	double speed = velocity.Length();
	if (speed > m_maxSpeed)
	{
		velocity *= (m_maxSpeed / speed);
		m_circleBody->SetLinearVelocity(velocity);
	}
}

