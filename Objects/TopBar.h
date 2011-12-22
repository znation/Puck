#include "Box2D\Box2D.h"
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
		DeviceContext *ctx,
		b2Vec2 topBarSize,
		b2Vec2 topBarPosition,
		WriteFactory *dwriteFactory);
	~TopBar();
	void ScoreGoal(int playerIdx);
	void Draw();
	void OnMouseDown(PointerEventArgs *args);
private:
	DeviceContext *m_ctx;
	Score *m_score[2];
	Scene *m_scene;
	Game *m_game;
	b2Vec2 m_size;
	b2Vec2 m_position;
	D2D1_ROUNDED_RECT m_menuButtonRect;
	IDWriteTextFormat *m_menuButtonTextFormat;
};
