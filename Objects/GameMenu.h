#include <wrl.h>
#include <d2d1_1.h>
#include <Box2D\Box2D.h>
#include <dwrite_1.h>
#include "Utility.h"

#pragma once

using namespace Microsoft::WRL;

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
	GameMenu(b2Vec2 viewportSize, Game *game, ComPtr<ID2D1DeviceContext> ctx, ComPtr<IDWriteFactory1> dwriteFactory);
	void Draw();
	void OnMouseDown(Windows::UI::Core::PointerEventArgs^ args);
private:
	b2Vec2 m_size;
	ComPtr<ID2D1DeviceContext> m_ctx;
	ComPtr<IDWriteFactory1> m_dwriteFactory;
	Game *m_game;
	D2D1_RECT_F m_rect;
	ComPtr<ID2D1SolidColorBrush> m_transparentBgBrush;
	ComPtr<ID2D1SolidColorBrush> m_solidBgBrush;
	ComPtr<ID2D1SolidColorBrush> m_buttonTextBrush;
	ComPtr<IDWriteTextFormat> m_buttonTextFormat;

	MenuButton m_buttons[MENU_BUTTON_COUNT];
};
