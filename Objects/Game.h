#include "Box2D\Box2D.h"
#include "Scene.h"
#include "GameMenu.h"
#include "Utility.h"

#pragma once

class Game
{
public:
	Game(b2Vec2 viewportSize, DeviceContext *ctx, WriteFactory *dwriteFactory);
	~Game();
	void Draw();
	void OnMouseMove(b2Vec2 p);
	void OnMouseDown(PointerEventArgs ^args);
	void Begin();
	void ShowMenu();
	void Resume();
	bool CanResume();
	void Resize(b2Vec2 viewportSize);
	
private:
	b2Vec2 m_size;
	b2World * m_world;
	Scene *m_scene;
	bool m_showMenu;
	GameMenu * m_menu;
	DeviceContext *m_ctx;
	WriteFactory *m_dwriteFactory;
};
