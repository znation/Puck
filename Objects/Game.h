#include "Box2D\Box2D.h"
#include <dwrite_1.h>
#include "Scene.h"
#include "GameMenu.h"
#include "Utility.h"

#pragma once

class Game
{
public:
	Game(b2Vec2 viewportSize, ComPtr<ID2D1RenderTarget> ctx, ComPtr<IDWriteFactory1> dwriteFactory);
	~Game();
	void Draw();
	void OnMouseMove(b2Vec2 p);
#ifdef WINRT
	void OnMouseDown(Windows::UI::Core::PointerEventArgs^ args);
#endif
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
	ComPtr<ID2D1RenderTarget> m_ctx;
	ComPtr<IDWriteFactory1> m_dwriteFactory;
};
