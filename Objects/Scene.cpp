#include "Scene.h"
#include "DirectXSample.h"
#include "D2DRenderer.h"
#include "b2UserData.h"

using namespace Microsoft::WRL;

Scene::Edge::Edge(b2Vec2 center_, b2Vec2 extents_, const char *e)
{
	center = center_;
	extents = extents_;
	extraInfo = e;
}

Scene::Scene(b2Vec2 viewportSize, ComPtr<ID2D1DeviceContext> ctx, b2World *world)
{
	int padding = 16;
	m_size = b2Vec2(viewportSize.x - (2 * padding),
		viewportSize.y - (2 * padding));
	m_position = b2Vec2(padding, padding);

	DX::ThrowIfFailed(ctx->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Cyan),
		&m_brush));

	DX::ThrowIfFailed(ctx->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(0.0784313725490196f,
		0.0784313725490196f,
		0.0784313725490196f)),
		&m_gridBrush));

	m_rect.rect.top = m_position.y;
	m_rect.rect.left = m_position.x;
	m_rect.rect.bottom = m_position.y + m_size.y;
	m_rect.rect.right = m_position.x + m_size.x;
	m_rect.radiusX = 10;
	m_rect.radiusY = 10;

	// Construct a border box to keep everything inside
	b2Vec2 horizontalEdgeSize = b2Vec2(m_size.x / 2.0, m_position.y / 2.0);
	b2Vec2 verticalEdgeSize = b2Vec2(m_position.x / 2.0, m_size.y / 2.0);
	Edge edges[] = {Edge(b2Vec2(m_position.x + (m_size.x / 2.0), (m_position.y / 2.0) + m_size.y), horizontalEdgeSize, "x"), // bottom
		Edge(b2Vec2(m_position.x + (m_size.x / 2.0), m_position.y / 2.0), horizontalEdgeSize, "x"), // top
		Edge(b2Vec2(m_position.x / 2.0, m_position.y + (m_size.y / 2.0)), verticalEdgeSize, "y"), // left
		Edge(b2Vec2(m_size.x + (m_position.x / 2.0), m_position.y + (m_size.y / 2.0)), verticalEdgeSize, "y")}; // right

	for (int i=0; i<4; i++)
	{
		Edge edge = edges[i];

		// Normalize to box coordinates
		edge.center.Set(pixelToBox(edge.center.x), pixelToBox(edge.center.y));
		edge.extents.Set(pixelToBox(edge.extents.x), pixelToBox(edge.extents.y));

		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(edge.center.x, edge.center.y);
		groundBodyDef.type = b2_staticBody;
		m_groundBoxBody = world->CreateBody(&groundBodyDef);
		b2PolygonShape groundBox;
		groundBox.SetAsBox(edge.extents.x, edge.extents.y);
		m_groundBoxBody->CreateFixture(&groundBox, 0.0f);
		m_groundBoxBody->SetUserData(new b2UserData(b2UserData_Edge, (void*)edge.extraInfo));
	}

	m_players[0] = new Player(m_size, m_position, ctx, 0, world, m_groundBoxBody);
	m_players[1] = new Player(m_size, m_position, ctx, 1, world, m_groundBoxBody);
	m_puck = new Puck(viewportSize, ctx, world);
}

void Scene::drawGrid(ComPtr<ID2D1DeviceContext> ctx)
{
	double verticalSquareCount = 25.0;
	int sideLength = floor(((double) m_size.y) / verticalSquareCount);

	D2D1_POINT_2F x1y1;
	D2D1_POINT_2F x2y2;

	// draw vertical lines
	int x = sideLength + m_position.x;
	while (x < (m_size.x + m_position.x - 1))
	{
		x1y1.x = x;
		x1y1.y = m_position.y + 1;
		x2y2.x = x;
		x2y2.y = m_size.y + m_position.y - 1;

		ctx->DrawLine(x1y1,
			x2y2,
			*(&m_gridBrush));

		x += sideLength;
	}
	x = sideLength + m_position.y;
	while (x < (m_size.y + m_position.y - 1))
	{
		x1y1.x = m_position.x + 1;
		x1y1.y = x;
		x2y2.x = m_size.x + m_position.x - 1;
		x2y2.y = x;

		ctx->DrawLine(x1y1,
			x2y2,
			*(&m_gridBrush));

		x += sideLength;
	}
}

void Scene::draw(ComPtr<ID2D1DeviceContext> ctx)
{
	ctx->DrawRoundedRectangle(&m_rect,
		m_brush.Get(),
		2.0f);

	drawGrid(ctx);

	for (int i=0; i<2; i++)
	{
		m_players[i]->draw(ctx);
	}

	m_puck->draw(ctx);
}

void Scene::move()
{
	m_puck->move();
	for (int i=0; i<2; i++)
	{
		m_players[i]->m_stick->move();
	}
}

void Scene::applyConstraints()
{
	m_puck->applyConstraints();
}

void Scene::detectCollisions()
{
	m_puck->detectCollisions();
}

void Scene::onMouseMoved(b2Vec2 p)
{
	for (int i=0; i<2; i++)
	{
		if (m_players[i] != nullptr &&
			m_players[i]->containsPoint(p))
		{
			m_players[i]->m_stick->updatePosition(p.x, p.y);
			return;
		}
	}
}
