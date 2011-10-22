#include <d2d1_1.h>
#include "Box2D\Box2D.h"
#include <dwrite_1.h>
#include "Score.h"
#include "Utility.h"

#pragma once

class Scene;
class Game;

class TopBar
{
public:
	TopBar(Scene *scene,
		Game *game,
		ComPtr<ID2D1DeviceContext> ctx,
		b2Vec2 topBarSize,
		b2Vec2 topBarPosition,
		ComPtr<IDWriteFactory1> dwriteFactory);
	~TopBar();
	void ScoreGoal(int playerIdx);
	void Draw();
#ifdef WINRT
	void OnMouseDown(Windows::UI::Core::PointerEventArgs^ args);
#endif
private:
	ComPtr<ID2D1DeviceContext> m_ctx;
	Score *m_score[2];
	Scene *m_scene;
	Game *m_game;
	b2Vec2 m_size;
	b2Vec2 m_position;
	D2D1_ROUNDED_RECT m_menuButtonRect;
	ComPtr<IDWriteTextFormat> m_menuButtonTextFormat;
};
