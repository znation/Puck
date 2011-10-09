#include "GameMenu.h"
#include "DirectXSample.h"
#include "Game.h"
#include "Utility.h"

GameMenu::GameMenu(b2Vec2 viewportSize, Game *game, ComPtr<ID2D1DeviceContext> ctx, ComPtr<IDWriteFactory1> dwriteFactory)
{
	m_size = viewportSize;
	m_game = game;
	m_ctx = ctx;
	m_dwriteFactory = dwriteFactory;
	
	m_rect.top = 0;
	m_rect.bottom = m_size.y;
	m_rect.left = 0;
	m_rect.right = m_size.x;

	int buttonRectWidth = 300;
	int buttonRectHeight = 200;
	m_buttonRect.top = (m_size.y / 2.0) - (buttonRectHeight / 2.0);
	m_buttonRect.bottom = (m_size.y / 2.0) + (buttonRectHeight / 2.0);
	m_buttonRect.left = (m_size.x / 2.0) - (buttonRectWidth / 2.0);
	m_buttonRect.right = (m_size.x / 2.0) + (buttonRectWidth / 2.0);

	DX::ThrowIfFailed(ctx->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black),
		&m_solidBgBrush));

	DX::ThrowIfFailed(ctx->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::White),
		&m_buttonTextBrush));

	DX::ThrowIfFailed(ctx->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(0.5, 0.5, 0.5, 0.5)),
		&m_transparentBgBrush));

	m_buttonText = L"New Game";
	m_buttonTextLength = 8;

	DX::ThrowIfFailed(
		m_dwriteFactory->CreateTextFormat(
		L"Segoe UI",
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		64.0f,
		L"en-US",
		&m_buttonTextFormat));

	DX::ThrowIfFailed(m_buttonTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
	DX::ThrowIfFailed(m_buttonTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));
}

void GameMenu::Draw()
{
	m_ctx->FillRectangle(
		&m_rect,
		m_transparentBgBrush.Get());

	m_ctx->FillRectangle(
		&m_buttonRect,
		m_solidBgBrush.Get());

	m_ctx->DrawText(
		m_buttonText,
		m_buttonTextLength,
		m_buttonTextFormat.Get(),
		&m_buttonRect,
		m_buttonTextBrush.Get());
}

void GameMenu::OnMouseDown(Windows::UI::Core::PointerEventArgs^ args)
{
	Windows::Foundation::Point pp = args->CurrentPoint->Position;
	b2Vec2 p = b2Vec2(pp.X, pp.Y);
	if (rectContainsPoint(m_buttonRect, p))
	{
		m_game->Begin();
	}
}
