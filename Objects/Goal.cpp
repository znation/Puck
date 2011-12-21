#include "Goal.h"
#include "b2UserData.h"
#include "Utility.h"
#include "Scene.h"

Goal::Goal(b2Vec2 sceneSize,
		   b2Vec2 scenePosition,
		   int playerIdx,
		   ComPtr<ID2D1DeviceContext> ctx,
		   b2World *world,
		   Scene *scene)
{
	m_ctx = ctx;
	m_scene = scene;
	m_playerIdx = playerIdx;
	m_size = b2Vec2(16.0f, sceneSize.y / 4.0f);
	m_position = b2Vec2(playerIdx == 0 ? scenePosition.x - (m_size.x / 2.0f) : 
		scenePosition.x + sceneSize.x - (m_size.x / 2.0f),
		scenePosition.y + ((sceneSize.y / 2.0f) - (m_size.y / 2.0f)));
	ThrowIfFailed(ctx->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::WhiteSmoke),
		&m_brush));

	m_rect.top = m_position.y;
	m_rect.left = m_position.x;
	m_rect.bottom = m_position.y + m_size.y;
	m_rect.right = m_position.x + m_size.x;

	b2Vec2 center, extents;
	center.x = pixelToBox(m_position.x + (m_size.x / 2.0f));
	center.y = pixelToBox(m_position.y + (m_size.y / 2.0f));
	extents.x = pixelToBox(m_size.x / 2.0f);
	extents.y = pixelToBox(m_size.y / 2.0f);

	b2BodyDef bodyDef;
	bodyDef.position.Set(center.x, center.y);
	bodyDef.type = b2_staticBody;
	m_goalBody = world->CreateBody(&bodyDef);
	b2PolygonShape goalBox;
	goalBox.SetAsBox(extents.x, extents.y);
	m_goalBody->CreateFixture(&goalBox, 0.0f);
	m_userData = new b2UserData(b2UserData_Goal, nullptr);
	m_goalBody->SetUserData(m_userData);
}

Goal::~Goal()
{
	delete m_userData;
}

void Goal::draw()
{
	m_ctx->FillRectangle(
		&m_rect,
		m_brush.Get());
}

void Goal::detectCollisions()
{
	b2ContactEdge *contactNode = m_goalBody->GetContactList();
	while (contactNode != nullptr)
	{
		b2Contact *contact = contactNode->contact;

		b2Fixture *fixtures[2];
		b2Body *bodies[2];
		int goalIdx = -1;
		int puckIdx = -1;

		fixtures[0] = contact->GetFixtureA();
		fixtures[1] = contact->GetFixtureB();
		for (int i = 0; i < 2; i++)
		{
			bodies[i] = fixtures[i]->GetBody();
			b2UserData *userData = (b2UserData*) bodies[i]->GetUserData();
			assert(userData != nullptr);
			if (userData->type == b2UserData_Puck)
			{
				puckIdx = i;
			}
			else if (userData->type == b2UserData_Goal)
			{
				goalIdx = i;
			}
		}

		if (goalIdx != -1 && puckIdx != -1 && contact->IsTouching())
		{
			m_scene->scoreGoal(m_playerIdx == 0 ? 1 : 0);
			return;
		}

		contactNode = contactNode->next;
	}
}