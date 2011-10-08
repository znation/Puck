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

Scene::Scene(b2Vec2 viewportSize, ComPtr<ID2D1DeviceContext> ctx, b2World *world, ComPtr<IDWriteFactory1> dwriteFactory)
{
	m_ctx = ctx;
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

	DX::ThrowIfFailed(
		dwriteFactory->CreateTextFormat(
		L"Segoe UI",
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		64.0f,
		L"en-US",
		&m_roundTimerTextFormat));

	DX::ThrowIfFailed(m_roundTimerTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
	DX::ThrowIfFailed(m_roundTimerTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));

	DX::ThrowIfFailed(ctx->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(0.0784313725490196f,
		0.0784313725490196f,
		0.0784313725490196f,
		0.8f)),
		&m_roundTimerRectBrush));

	m_rect.rect.top = m_position.y;
	m_rect.rect.left = m_position.x;
	m_rect.rect.bottom = m_position.y + m_size.y;
	m_rect.rect.right = m_position.x + m_size.x;
	m_rect.radiusX = 10;
	m_rect.radiusY = 10;

	int roundTimerRectWidth = 200;
	int roundTimerRectHeight = 120;
	m_roundTimerRect.top = m_position.y + (m_size.y / 2.0) - (roundTimerRectHeight / 2.0);
	m_roundTimerRect.bottom = m_position.y + (m_size.y / 2.0) + (roundTimerRectHeight / 2.0);
	m_roundTimerRect.left = m_position.x + (m_size.x / 2.0) - (roundTimerRectWidth / 2.0);
	m_roundTimerRect.right = m_position.x + (m_size.x / 2.0) - (roundTimerRectWidth / 2.0);

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

	m_players[0] = new Player(m_size, m_position, ctx, 0, world, m_groundBoxBody, dwriteFactory, this);
	m_players[1] = new Player(m_size, m_position, ctx, 1, world, m_groundBoxBody, dwriteFactory, this);
	m_puck = new Puck(viewportSize, ctx, world);

	beginRound();
}

void Scene::win(int playerIdx)
{
	m_frozen = true;
	m_gameOver = true;
	m_players[playerIdx]->showWinnerText();
}

void Scene::reset()
{
	for (int i=0; i<2; i++)
	{
		m_players[i]->reset();
	}
	m_puck->reset();

	beginRound();
}

void Scene::beginRound()
{
	m_frozen = true;
	m_beginTime = GetTickCount64();
}

void Scene::drawRoundTimer()
{
	if (m_frozen)
	{
		ULONGLONG currentTime = GetTickCount64();
		ULONGLONG diffSeconds = (currentTime - m_beginTime) / 1000;
		const wchar_t *timerText;
		size_t timerTextLength;
		switch (diffSeconds)
		{
		case 0:
			timerText = L"3";
			timerTextLength = 1;
			break;
		case 1:
			timerText = L"2";
			timerTextLength = 1;
			break;
		case 2:
			timerText = L"1";
			timerTextLength = 1;
			break;
		case 3:
			timerText = L"GO!";
			timerTextLength = 3;
			break;
		default: // 4 or more seconds
			m_frozen = false;
			return;
		}

		m_ctx->FillRectangle(
			m_roundTimerRect,
			m_roundTimerRectBrush.Get());

		m_ctx->DrawText(
			timerText,
			timerTextLength,
			m_roundTimerTextFormat.Get(),
			&(m_rect.rect),
			m_brush.Get());
	}
}

void Scene::drawGrid()
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

		m_ctx->DrawLine(x1y1,
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

		m_ctx->DrawLine(x1y1,
			x2y2,
			*(&m_gridBrush));

		x += sideLength;
	}
}

void Scene::draw()
{
	m_ctx->DrawRoundedRectangle(&m_rect,
		m_brush.Get(),
		2.0f);

	drawGrid();

	for (int i=0; i<2; i++)
	{
		m_players[i]->draw();
	}

	m_puck->draw();

	drawRoundTimer();
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
	for (int i=0; i<2; i++)
	{
		if (m_players[i] != nullptr)
		{
			m_players[i]->detectCollisions();
		}
	}
}

void Scene::onMouseMoved(b2Vec2 p)
{
	if (m_frozen)
	{
		return;
	}

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
