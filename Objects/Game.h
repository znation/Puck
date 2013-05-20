#pragma once

#include "Box2D\Box2D.h"
#include "Scene.h"
#include "GameMenu.h"
#include "Utility.h"

#include <memory>

class Game
{
public:
	Game(b2Vec2 viewportSize, DeviceContext *ctx, WriteFactory *dwriteFactory);
	void Draw();
	void OnMouseMove(b2Vec2 p);
	void OnMouseDown(b2Vec2 p, bool left, bool right);
	void Begin();
	void ShowMenu();
	void Resume();
	bool CanResume();
	void Resize(b2Vec2 viewportSize);
	
private:
	b2Vec2 m_size;
	std::unique_ptr<b2World> m_world;
	std::unique_ptr<Scene> m_scene; // must be defined after m_world since it has a reference to it
	bool m_showMenu;
	std::unique_ptr<GameMenu> m_menu;
	DeviceContext *m_ctx;
	WriteFactory *m_dwriteFactory;
};
