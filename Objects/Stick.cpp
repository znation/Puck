#include "Stick.h"
#include "Utility.h"
#include "b2UserData.h"

Stick::Stick(b2Vec2 playerSize, b2Vec2 playerPosition, b2World *world, b2Body *groundBody)
{
	m_radius = 32.0f;
	m_startingPosition = m_position = b2Vec2(playerPosition.x + (playerSize.x/2.0f),
		playerPosition.y + (playerSize.y/2.0f));
	m_nextPosition = m_position;

	m_innerCircleWidth = floor(m_radius / 10.0f);
	m_outerCircleWidth = floor(m_radius / 15.0f);
	m_innerCircleWidth = max(1, m_innerCircleWidth);
	m_outerCircleWidth = max(m_innerCircleWidth+1, m_outerCircleWidth);

	b2BodyDef circleBd;
	circleBd.position.Set(pixelToBox(m_position.x), pixelToBox(m_position.y));
	circleBd.type = b2_dynamicBody;
	circleBd.fixedRotation = true;
	circleBd.bullet = true;
	m_circleBody = world->CreateBody(&circleBd);
	m_circleBody->SetUserData(new b2UserData(b2UserData_Stick, nullptr));

	b2CircleShape circleSd;
	circleSd.m_type = b2Shape::Type::e_circle;
	circleSd.m_radius = pixelToBox(m_radius);

	b2FixtureDef circleF;
	circleF.shape = &circleSd;
	circleF.density = 0.1f;
	circleF.friction = 0.0f;
	circleF.restitution = 1.0f;
	m_circleBody->CreateFixture(&circleF);

	b2MouseJointDef mousedef;
	mousedef.bodyA = groundBody;
	mousedef.bodyB = m_circleBody;
	mousedef.target.Set(circleBd.position.x, circleBd.position.y);
	mousedef.collideConnected = true;
	mousedef.maxForce = 10000 * m_circleBody->GetMass();
	mousedef.frequencyHz = 100;
	mousedef.dampingRatio = 0;
	m_boxMouseJoint = (b2MouseJoint*) world->CreateJoint(&mousedef);
}

void Stick::reset()
{
	updatePosition(m_startingPosition.x, m_startingPosition.y);
}

void Stick::updateEllipse()
{
	float innerCircleRadius = m_radius / 2.0f;
	m_innerEllipse.point.x = m_position.x;
	m_innerEllipse.point.y = m_position.y;
	m_innerEllipse.radiusX = innerCircleRadius;
	m_innerEllipse.radiusY = innerCircleRadius;

	m_outerEllipse.point.x = m_position.x;
	m_outerEllipse.point.y = m_position.y;
	m_outerEllipse.radiusX = m_radius;
	m_outerEllipse.radiusY = m_radius;
}

void Stick::updatePosition(float x, float y)
{
	m_nextPosition = b2Vec2(x, y);
}

void Stick::move()
{
	m_boxMouseJoint->SetTarget(b2Vec2(pixelToBox(m_nextPosition.x), pixelToBox(m_nextPosition.y)));
	b2Vec2 p = m_circleBody->GetPosition();
	m_position.Set(boxToPixel(p.x), boxToPixel(p.y));
	updateEllipse();
}

void Stick::draw(ComPtr<ID2D1DeviceContext> ctx, ComPtr<ID2D1SolidColorBrush> brush)
{
	ctx->DrawEllipse(
		&m_outerEllipse,
		brush.Get(),
		m_outerCircleWidth);

	ctx->DrawEllipse(
		&m_innerEllipse,
		brush.Get(),
		m_innerCircleWidth);
}
