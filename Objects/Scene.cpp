#include "Scene.h"
#include "Utility.h"
#include "b2UserData.h"
#include "Game.h"

ComPtr<ID2D1SolidColorBrush> Scene::Cyan;

Scene::Edge::Edge(b2Vec2 center_, b2Vec2 extents_, const char *e)
{
	center = center_;
	extents = extents_;
	extraInfo = e;
}

Scene::Scene(b2Vec2 viewportSize,
			 ComPtr<ID2D1RenderTarget> ctx,
			 b2World *world,
			 ComPtr<IDWriteFactory1> dwriteFactory,
			 Game *game)
{
	m_frozen = false;
	m_gameOver = false;

	m_ctx = ctx;

	float padding = 16.0f;
	b2Vec2 topBarSize = b2Vec2(viewportSize.x - (2 * padding), 32.0f);
	b2Vec2 topBarPosition = b2Vec2(padding, padding);

	m_size = b2Vec2(viewportSize.x - (2 * padding),
		viewportSize.y - ((2 * padding) + topBarSize.y));
	m_position = b2Vec2(padding, padding + topBarPosition.y + topBarSize.y);

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

	m_rect.rect.top = m_position.y;
	m_rect.rect.left = m_position.x;
	m_rect.rect.bottom = m_position.y + m_size.y;
	m_rect.rect.right = m_position.x + m_size.x;
	m_rect.radiusX = 10;
	m_rect.radiusY = 10;

	// Construct a border box to keep everything inside
	b2Vec2 horizontalEdgeSize = b2Vec2(m_size.x / 2.0f, padding / 2.0f);
	b2Vec2 verticalEdgeSize = b2Vec2(padding / 2.0f, m_size.y / 2.0f);
	Edge edges[] = {Edge(b2Vec2(m_position.x + (m_size.x / 2.0f), m_position.y + m_size.y + (padding / 2.0f)), horizontalEdgeSize, "x"), // bottom
		Edge(b2Vec2(m_position.x + (m_size.x / 2.0f), m_position.y - (padding / 2.0f)), horizontalEdgeSize, "x"), // top
		Edge(b2Vec2(padding / 2.0f, m_position.y + (m_size.y / 2.0f)), verticalEdgeSize, "y"), // left
		Edge(b2Vec2(m_position.x + m_size.x + (padding / 2.0f), m_position.y + (m_size.y / 2.0f)), verticalEdgeSize, "y")}; // right

	b2Body * groundBoxBody;
	for (int i=0; i<4; i++)
	{
		Edge edge = edges[i];

		// Normalize to box coordinates
		edge.center.Set(pixelToBox(edge.center.x), pixelToBox(edge.center.y));
		edge.extents.Set(pixelToBox(edge.extents.x), pixelToBox(edge.extents.y));

		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(edge.center.x, edge.center.y);
		groundBodyDef.type = b2_staticBody;
		groundBoxBody = world->CreateBody(&groundBodyDef);
		b2PolygonShape groundBox;
		groundBox.SetAsBox(edge.extents.x, edge.extents.y);
		groundBoxBody->CreateFixture(&groundBox, 0.0f);
		m_userData[i] = new b2UserData(b2UserData_Edge, (void*)edge.extraInfo);
		groundBoxBody->SetUserData(m_userData[i]);
	}

	m_players[0] = new Player(m_size, m_position, ctx, 0, world, groundBoxBody, dwriteFactory, this);
	m_players[1] = new Player(m_size, m_position, ctx, 1, world, groundBoxBody, dwriteFactory, this);
	m_puck = new Puck(viewportSize, ctx, world);
	m_topBar = new TopBar(this, game, ctx, topBarSize, topBarPosition, dwriteFactory);

	beginRound();
}

Scene::~Scene()
{
	for (int i=0; i<4; i++)
	{
		delete m_userData[i];
	}
	for (int i=0; i<2; i++)
	{
		delete m_players[i];
	}
	delete m_puck;
	delete m_topBar;
}

void Scene::win(int playerIdx)
{
	m_frozen = true;
	m_gameOver = true;
	m_players[playerIdx]->showWinnerText();
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
		m_players[i]->reset();
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
		return false;
	}

	m_ctx->DrawText(
		timerText,
		timerTextLength,
		m_roundTimerTextFormat.Get(),
		&(m_rect.rect),
		Scene::Cyan.Get());
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
		Scene::Cyan.Get(),
		2.0f);

	drawGrid();

	m_topBar->Draw();

	for (int i=0; i<2; i++)
	{
		m_players[i]->draw();
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

#ifdef WINRT
void Scene::OnMouseDown(Windows::UI::Core::PointerEventArgs^ args)
{
	m_topBar->OnMouseDown(args);
}
#endif
