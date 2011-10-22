#include <Box2D\Box2D.h>
#include <dwrite_1.h>
#include "Utility.h"

#pragma once

class Game;

class MenuButton
{
public:
	b2Vec2 Size;
	b2Vec2 Position;
	D2D1_ROUNDED_RECT RoundedRect;
	size_t TextLength;
	const wchar_t *Text;
};

class GameMenu
{
public:
	GameMenu(b2Vec2 viewportSize, Game *game, ComPtr<ID2D1RenderTarget> ctx, ComPtr<IDWriteFactory1> dwriteFactory);
	void Draw();
#ifdef WINRT
	void OnMouseDown(Windows::UI::Core::PointerEventArgs^ args);
#endif
private:
	b2Vec2 m_size;
	ComPtr<ID2D1RenderTarget> m_ctx;
	ComPtr<IDWriteFactory1> m_dwriteFactory;
	Game *m_game;
	D2D1_RECT_F m_rect;
	ComPtr<ID2D1SolidColorBrush> m_transparentBgBrush;
	ComPtr<ID2D1SolidColorBrush> m_solidBgBrush;
	ComPtr<ID2D1SolidColorBrush> m_buttonTextBrush;
	ComPtr<IDWriteTextFormat> m_buttonTextFormat;

	MenuButton m_buttons[MENU_BUTTON_COUNT];
};
