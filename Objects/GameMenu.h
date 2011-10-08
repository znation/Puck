#include <wrl.h>
#include <d2d1_1.h>
#include <Box2D\Box2D.h>
#include <dwrite_1.h>

#pragma once

using namespace Microsoft::WRL;

class Game;

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
	D2D1_RECT_F m_buttonRect;

	ComPtr<ID2D1SolidColorBrush> m_transparentBgBrush;
	ComPtr<ID2D1SolidColorBrush> m_solidBgBrush;
	ComPtr<ID2D1SolidColorBrush> m_buttonTextBrush;

	ComPtr<IDWriteTextFormat> m_buttonTextFormat;
	size_t m_buttonTextLength;
	const wchar_t *m_buttonText;
};
