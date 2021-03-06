#include "pch.h"
#include "Scene.h"
#include "Utility.h"
#include "b2UserData.h"
#include "Game.h"

SolidColorBrush * Scene::Cyan;

Scene::Edge::Edge() {}

void Scene::Edge::Initialize(b2Vec2 center_, b2Vec2 extents_, const char *e)
{
	center = center_;
	extents = extents_;
	extraInfo = e;
}

Scene::Scene(b2Vec2 viewportSize,
			 DeviceContext *ctx,
			 b2World *world,
			 WriteFactory *dwriteFactory,
			 Game *game)
{
	m_world = world;
	m_frozen = false;
	m_gameOver = false;

	m_ctx = ctx;

	ThrowIfFailed(ctx->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Cyan),
		&Scene::Cyan));

	ThrowIfFailed(ctx->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(0.0784313725490196f,
		0.0784313725490196f,
		0.0784313725490196f)),
		&m_gridBrush));

	ThrowIfFailed(
		dwriteFactory->CreateTextFormat(
		L"Segoe UI",
		nullptr,
		DWRITE_FONT_WEIGHT_EXTRA_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		128.0f,
		L"en-US",
		&m_roundTimerTextFormat));

	ThrowIfFailed(m_roundTimerTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
	ThrowIfFailed(m_roundTimerTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));
	
	// Construct a border box to keep everything inside
	for (int i=0; i<4; i++)
	{
		m_groundBoxBody[i] = nullptr;
	}

	m_topBar = std::unique_ptr<TopBar>(new TopBar(this, game, ctx, dwriteFactory));

	Resize(viewportSize);

	m_rect.rect.top = m_position.y;
	m_rect.rect.left = m_position.x;
	m_rect.rect.bottom = m_position.y + m_size.y;
	m_rect.rect.right = m_position.x + m_size.x;
	m_rect.radiusX = 0.0125f * viewportSize.y;
	m_rect.radiusY = 0.0125f * viewportSize.y;
	
	m_players.emplace_back(m_size, m_position, ctx, 0, world, m_groundBoxBody[3], dwriteFactory, this);
	m_players.emplace_back(m_size, m_position, ctx, 1, world, m_groundBoxBody[3], dwriteFactory, this);
	m_puck = std::unique_ptr<::Puck::Puck>(new Puck::Puck(viewportSize, ctx, world));
	
	beginRound();
}

void Scene::win(int playerIdx)
{
	m_frozen = true;
	m_gameOver = true;
	m_players[playerIdx].showWinnerText();
	resetCore();
}

void Scene::scoreGoal(int playerIdx)
{
	m_topBar->ScoreGoal(playerIdx);
}

void Scene::resetCore()
{
	for (int i=0; i<2; i++)
	{
		m_players[i].reset();
	}
	m_puck->reset();
}

void Scene::reset()
{
	resetCore();
	beginRound();
}

void Scene::beginRound()
{
	m_frozen = true;
	m_beginTime = GetTickCount64();
}

bool Scene::drawRoundTimer()
{
	if (!m_frozen || m_gameOver)
	{
		return false;
	}

	ULONGLONG currentTime = GetTickCount64();
	ULONGLONG diffSeconds = (currentTime - m_beginTime) / 1000;
	const wchar_t *timerText;
	UINT32 timerTextLength;
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
		return false;
	}

	m_ctx->DrawText(
		timerText,
		timerTextLength,
		m_roundTimerTextFormat,
		&(m_rect.rect),
		Scene::Cyan);
	return true;
}

void Scene::drawGrid()
{
	float verticalSquareCount = 25.0;
	float sideLength = floor(m_size.y / verticalSquareCount);

	D2D1_POINT_2F x1y1;
	D2D1_POINT_2F x2y2;

	// draw vertical lines
	float x = sideLength + m_position.x;
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
		Scene::Cyan,
		2.0f);

	drawGrid();

	m_topBar->Draw();

	for (int i=0; i<2; i++)
	{
		m_players[i].draw();
	}

	if (!drawRoundTimer())
	{
		m_puck->draw();
	}
}

void Scene::move()
{
	m_puck->move();
	for (int i=0; i<2; i++)
	{
		m_players[i].m_stick->move();
	}
}

void Scene::applyConstraints()
{
	m_puck->applyConstraints();
}

void Scene::detectCollisions()
{
	for (auto &player : m_players)
	{
		player.detectCollisions();
	}
}

void Scene::onMouseMoved(b2Vec2 p)
{
	if (m_frozen)
	{
		return;
	}

	for (auto &player : m_players)
	{
		if (player.containsPoint(p))
		{
			player.m_stick->updatePosition(p.x, p.y);
			return;
		}
	}
}

void Scene::OnMouseDown(b2Vec2 p)
{
	if (m_topBar != nullptr)
	{
		m_topBar->OnMouseDown(p);
	}
}

void Scene::Resize(b2Vec2 viewportSize)
{
	float padding = 0.02f * viewportSize.y;
	b2Vec2 topBarSize = b2Vec2(viewportSize.x - (2 * padding), 0.04f * viewportSize.y);
	b2Vec2 topBarPosition = b2Vec2(padding, padding);

	m_size = b2Vec2(viewportSize.x - (2.0f * padding),
		viewportSize.y - ((2.0f * padding) + topBarSize.y));
	m_position = b2Vec2(padding, padding + topBarPosition.y + topBarSize.y);

	b2Vec2 horizontalEdgeSize = b2Vec2(m_size.x / 2.0f, padding / 2.0f);
	b2Vec2 verticalEdgeSize = b2Vec2(padding / 2.0f, m_size.y / 2.0f);
	m_edges[0].Initialize(b2Vec2(m_position.x + (m_size.x / 2.0f), m_position.y + m_size.y + (padding / 2.0f)), horizontalEdgeSize, "x");
	m_edges[1].Initialize(b2Vec2(m_position.x + (m_size.x / 2.0f), m_position.y - (padding / 2.0f)), horizontalEdgeSize, "x");
	m_edges[2].Initialize(b2Vec2(padding / 2.0f, m_position.y + (m_size.y / 2.0f)), verticalEdgeSize, "y");
	m_edges[3].Initialize(b2Vec2(m_position.x + m_size.x + (padding / 2.0f), m_position.y + (m_size.y / 2.0f)), verticalEdgeSize, "y");

	m_userData.clear();
	for (int i=0; i<4; i++)
	{
		if (m_groundBoxBody[i] != nullptr)
		{
			m_world->DestroyBody(m_groundBoxBody[i]);
			m_groundBoxBody[i] = nullptr;
		}

		Edge edge = m_edges[i];

		// Normalize to box coordinates
		edge.center.Set(pixelToBox(edge.center.x), pixelToBox(edge.center.y));
		edge.extents.Set(pixelToBox(edge.extents.x), pixelToBox(edge.extents.y));

		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(edge.center.x, edge.center.y);
		groundBodyDef.type = b2_staticBody;
		m_groundBoxBody[i] = m_world->CreateBody(&groundBodyDef);
		b2PolygonShape groundBox;
		groundBox.SetAsBox(edge.extents.x, edge.extents.y);
		m_groundBoxBody[i]->CreateFixture(&groundBox, 0.0f);
		m_userData.push_back(b2UserData(b2UserData_Edge, (void*)edge.extraInfo));
		m_groundBoxBody[i]->SetUserData(&(m_userData[i]));
	}

	m_topBar->Resize(topBarSize, topBarPosition);
}