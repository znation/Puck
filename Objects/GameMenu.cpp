#include "GameMenu.h"
#include "Game.h"
#include "Utility.h"

GameMenu::GameMenu(b2Vec2 viewportSize, Game * game, ComPtr<ID2D1DeviceContext> ctx, ComPtr<IDWriteFactory1> dwriteFactory)
{
	m_size = viewportSize;
	m_game = game;
	m_ctx = ctx;
	m_dwriteFactory = dwriteFactory;

	m_rect.top = 0;
	m_rect.bottom = m_size.y;
	m_rect.left = 0;
	m_rect.right = m_size.x;

	float buttonWidth = 300;
	float buttonHeight = 200;
	float totalButtonWidth = buttonWidth * MENU_BUTTON_COUNT;
	for (int i=0; i<MENU_BUTTON_COUNT; i++)
	{
		m_buttons[i].Size = b2Vec2(300, 200);
		m_buttons[i].Position = b2Vec2(((m_size.x - totalButtonWidth) / 2.0f) + (i * buttonWidth), (m_size.y / 2.0f) - (buttonHeight / 2.0f));
		m_buttons[i].RoundedRect.radiusX = 10;
		m_buttons[i].RoundedRect.radiusY = 10;
		m_buttons[i].RoundedRect.rect.left = m_buttons[i].Position.x;
		m_buttons[i].RoundedRect.rect.top = m_buttons[i].Position.y;
		m_buttons[i].RoundedRect.rect.right = m_buttons[i].Position.x + m_buttons[i].Size.x;
		m_buttons[i].RoundedRect.rect.bottom = m_buttons[i].Position.y + m_buttons[i].Size.y;
	}

	m_buttons[0].Text = L"New Game";
	m_buttons[0].TextLength = 8;
	m_buttons[1].Text = L"Resume";
	m_buttons[1].TextLength = 6;

	ThrowIfFailed(ctx->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black),
		&m_solidBgBrush));

	ThrowIfFailed(ctx->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::White),
		&m_buttonTextBrush));

	ThrowIfFailed(ctx->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(0.5, 0.5, 0.5, 0.5)),
		&m_transparentBgBrush));

	ThrowIfFailed(
		m_dwriteFactory->CreateTextFormat(
		L"Segoe UI",
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		64.0f,
		L"en-US",
		&m_buttonTextFormat));

	ThrowIfFailed(m_buttonTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
	ThrowIfFailed(m_buttonTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));
}

void GameMenu::Draw()
{
	m_ctx->FillRectangle(
		&m_rect,
		m_transparentBgBrush.Get());

	for (int i=0; i<MENU_BUTTON_COUNT; i++)
	{
		ComPtr<ID2D1SolidColorBrush> brush = m_solidBgBrush;
		if (i == 1 && !m_game->CanResume())
		{
			brush = m_transparentBgBrush;
		}

		m_ctx->FillRoundedRectangle(
			&(m_buttons[i].RoundedRect),
			brush.Get());

		m_ctx->DrawText(
			m_buttons[i].Text,
			m_buttons[i].TextLength,
			m_buttonTextFormat.Get(),
			&m_buttons[i].RoundedRect.rect,
			m_buttonTextBrush.Get());
	}
}

void GameMenu::OnMouseDown(Windows::UI::Core::PointerEventArgs^ args)
{
	Windows::Foundation::Point pp = args->CurrentPoint->Position;
	b2Vec2 p = b2Vec2(pp.X, pp.Y);
	if (rectContainsPoint(m_buttons[0].RoundedRect.rect, p))
	{
		m_game->Begin();
	}
	else if (m_game->CanResume() && rectContainsPoint(m_buttons[1].RoundedRect.rect, p))
	{
		m_game->Resume();
	}
}
