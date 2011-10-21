#include <d2d1_1.h>
#include "Box2D\Box2D.h"
#include <dwrite_1.h>
#include "Scene.h"
#include "GameMenu.h"

#pragma once

class Game
{
public:
	Game(b2Vec2 viewportSize, ComPtr<ID2D1DeviceContext> ctx, ComPtr<IDWriteFactory1> dwriteFactory);
	~Game();
	void Draw();
	void OnMouseMove(b2Vec2 p);
	void OnMouseDown(Windows::UI::Core::PointerEventArgs^ args);
	void Begin();
	void ShowMenu();
	void Resume();
	bool CanResume();
	
private:
	b2Vec2 m_size;
	b2World * m_world;
	Scene *m_scene;
	bool m_showMenu;
	GameMenu * m_menu;
	ComPtr<ID2D1DeviceContext> m_ctx;
	ComPtr<IDWriteFactory1> m_dwriteFactory;
};
