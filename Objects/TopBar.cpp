#include "TopBar.h"
#include "Scene.h"
#include "Game.h"
#include "Utility.h"

TopBar::TopBar(Scene *scene,
			   Game *game,
			   ComPtr<ID2D1DeviceContext> ctx,
			   b2Vec2 topBarSize,
			   b2Vec2 topBarPosition,
			   ComPtr<IDWriteFactory1> dwriteFactory)
{
	m_scene = scene;
	m_game = game;
	m_ctx = ctx;
	m_size = topBarSize;
	m_position = topBarPosition;

	b2Vec2 buttonAreaSize = b2Vec2(300, topBarSize.y);
	b2Vec2 buttonAreaPosition = b2Vec2(m_position.x + (m_size.x / 2.0f) - (buttonAreaSize.x / 2.0f),
		topBarPosition.y);

	m_menuButtonRect.radiusX = 10;
	m_menuButtonRect.radiusY = 10;
	m_menuButtonRect.rect.top = buttonAreaPosition.y;
	m_menuButtonRect.rect.left = buttonAreaPosition.x;
	m_menuButtonRect.rect.bottom = buttonAreaPosition.y + buttonAreaSize.y;
	m_menuButtonRect.rect.right = buttonAreaPosition.x + buttonAreaSize.x;

	for (int i=0; i<2; i++)
	{
		b2Vec2 scoreSize = b2Vec2((topBarSize.x - buttonAreaSize.x) / 2.0f, topBarSize.y);
		b2Vec2 scorePosition = b2Vec2(i == 0 ?
			topBarPosition.x :
		topBarPosition.x + scoreSize.x + buttonAreaSize.x,
			topBarPosition.y);
		m_score[i] = new Score(scoreSize, scorePosition, i, ctx);
	}

	ThrowIfFailed(
		dwriteFactory->CreateTextFormat(
		L"Segoe UI",
		nullptr,
		DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		32.0f,
		L"en-US",
		&m_menuButtonTextFormat));

	ThrowIfFailed(m_menuButtonTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
	ThrowIfFailed(m_menuButtonTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));
}

TopBar::~TopBar()
{
	for (int i=0; i<2; i++)
	{
		delete m_score[i];
	}
}

void TopBar::Draw()
{
	for (int i=0; i<2; i++)
	{
		m_score[i]->draw();
	}

	// Draw menu button
	m_ctx->DrawRoundedRectangle(
		&m_menuButtonRect,
		Scene::Cyan.Get());

	m_ctx->DrawText(
		L"MENU",
		4,
		m_menuButtonTextFormat.Get(),
		&(m_menuButtonRect.rect),
		Scene::Cyan.Get());
}

void TopBar::ScoreGoal(int playerIdx)
{
	m_score[playerIdx]->increment();
	int score = m_score[playerIdx]->getScore();
	if (score == MAX_SCORE)
	{
		m_scene->win(playerIdx);
	}
	else
	{
		m_scene->reset();
	}
}

void TopBar::OnMouseDown(Windows::UI::Core::PointerEventArgs^ args)
{
	b2Vec2 p = b2Vec2(args->CurrentPoint->Position.X, args->CurrentPoint->Position.Y);
	if (rectContainsPoint(m_menuButtonRect.rect, p))
	{
		m_game->ShowMenu();
	}
}
