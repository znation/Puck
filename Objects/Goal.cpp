#include "Goal.h"
#include "DirectXSample.h"
#include "b2UserData.h"
#include "D2DRenderer.h"

Goal::Goal(b2Vec2 playerSize, b2Vec2 playerPosition, int playerIdx, ComPtr<ID2D1DeviceContext> ctx, b2World *world)
{
	m_size = b2Vec2(16.0, playerSize.y / 8);
	m_position = b2Vec2(playerIdx == 0 ? playerPosition.x - (m_size.x / 2.0) : 
		playerPosition.x + playerSize.x - (m_size.x / 2.0),
		playerPosition.y + ((playerSize.y / 2) - (m_size.y / 2.0)));
	DX::ThrowIfFailed(ctx->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::WhiteSmoke),
		&m_brush));

	m_rect.top = m_position.y;
	m_rect.left = m_position.x;
	m_rect.bottom = m_position.y + m_size.y;
	m_rect.right = m_position.x + m_size.x;

	b2Vec2 center, extents;
	center.x = pixelToBox(m_position.x + (m_size.x / 2.0));
	center.y = pixelToBox(m_position.y + (m_size.y / 2.0));
	extents.x = pixelToBox(m_size.x / 2.0);
	extents.y = pixelToBox(m_size.y / 2.0);

	b2BodyDef bodyDef;
	bodyDef.position.Set(center.x, center.y);
	bodyDef.type = b2_staticBody;
	m_goalBody = world->CreateBody(&bodyDef);
	b2PolygonShape goalBox;
	goalBox.SetAsBox(extents.x, extents.y);
	m_goalBody->CreateFixture(&goalBox, 0.0f);
	m_goalBody->SetUserData(new b2UserData(b2UserData_Goal, nullptr));
}

void Goal::draw(ComPtr<ID2D1DeviceContext> ctx)
{
	ctx->FillRectangle(
		&m_rect,
		m_brush.Get());
}

void Goal::detectCollisions()
{
	b2ContactEdge *contactNode = m_goalBody->GetContactList();
	while (contactNode != nullptr)
	{
		b2Contact *contact = contactNode->contact;

		b2Fixture *otherShape = (contact->GetFixtureA()->GetBody() == m_goalBody) ? 
			contact->GetFixtureB() :
		contact->GetFixtureA();

		b2Body *otherBody = otherShape->GetBody();

		if (otherBody->GetUserData() != nullptr)
		{
			if (((b2UserData*)(otherBody->GetUserData()))->type == b2UserData_Puck)
			{
				//assert(false);
			}
		}

		contactNode = contactNode->next;
	}
}