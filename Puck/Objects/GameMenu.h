#include <Box2D\Box2D.h>
#include "Utility.h"

#pragma once

using namespace Windows::UI::Core;

class Game;

class MenuButton
{
public:
	b2Vec2 Size;
	b2Vec2 Position;
	D2D1_ROUNDED_RECT RoundedRect;
	UINT32 TextLength;
	const wchar_t *Text;
};

class GameMenu
{
public:
	GameMenu(Game *game, DeviceContext *ctx, WriteFactory *dwriteFactory);
	void Draw();
	void OnMouseDown(b2Vec2 p);
	void Resize(b2Vec2 viewportSize);

private:
	b2Vec2 m_size;
	DeviceContext *m_ctx;
	WriteFactory *m_dwriteFactory;
	Game *m_game;
	D2D1_RECT_F m_rect;
	SolidColorBrush *m_transparentBgBrush;
	SolidColorBrush *m_solidBgBrush;
	SolidColorBrush *m_buttonTextBrush;
	IDWriteTextFormat *m_buttonTextFormat;

	MenuButton m_buttons[MENU_BUTTON_COUNT];
};
