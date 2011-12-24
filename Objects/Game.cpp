#include "Game.h"

Game::Game(b2Vec2 viewportSize, DeviceContext *ctx, WriteFactory *dwriteFactory)
{	
	m_size = viewportSize;
	m_ctx = ctx;
	m_dwriteFactory = dwriteFactory;
	m_menu = new GameMenu(viewportSize, this, ctx, dwriteFactory);
	m_scene = nullptr;
	m_world = nullptr;
}

Game::~Game()
{
	if (m_menu != NULL)
		delete m_menu;

	if (m_world != NULL)
		delete m_world;

	if (m_scene != NULL)
		delete m_scene;
}

void Game::Begin()
{
	m_showMenu = false;

	// Tear down existing game
	if (m_world != nullptr || m_scene != nullptr)
	{
		assert(m_world != nullptr);
		assert(m_scene != nullptr);
		delete m_world;
		delete m_scene;
	}

	// Initialize Box2D world
	m_world = new b2World(b2Vec2(0,0));

	m_scene = new Scene(m_size,
		m_ctx,
		m_world,
		m_dwriteFactory,
		this);
}

void Game::Draw()
{
	if (m_scene != nullptr)
	{
		if (!m_showMenu)
		{
			m_scene->applyConstraints();
			// TODO use real measured framerate here instead of hardcoded ideal framerate
			m_world->Step(1.0f / FRAMERATE, 8, 3);
			m_scene->detectCollisions();
			m_scene->move();
		}
		m_scene->draw();
	}
	if (m_showMenu)
	{
		m_menu->Draw();
	}
}

void Game::OnMouseMove(b2Vec2 p)
{
	if (!m_showMenu && m_scene != nullptr)
	{
		m_scene->onMouseMoved(p);
	}
}

void Game::OnMouseDown(PointerEventArgs *args)
{
	if (args->CurrentPoint->Properties->IsRightButtonPressed)
	{
		m_showMenu = !m_showMenu;
	}
	else if (args->CurrentPoint->Properties->IsLeftButtonPressed)
	{
		if (m_showMenu)
		{
			m_menu->OnMouseDown(args);
		}
		else
		{
			m_scene->OnMouseDown(args);
		}
	}
}

void Game::ShowMenu()
{
	m_showMenu = true;
}

void Game::Resume()
{
	m_showMenu = false;
}

bool Game::CanResume()
{
	return m_scene != nullptr;
}