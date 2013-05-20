#include "pch.h"
#include "Game.h"

Game::Game(b2Vec2 viewportSize, DeviceContext *ctx, WriteFactory *dwriteFactory)
	:	m_size(viewportSize),
		m_ctx(ctx),
		m_dwriteFactory(dwriteFactory)
{	
	m_menu = std::unique_ptr<GameMenu>(new GameMenu(this, ctx, dwriteFactory));
	Resize(viewportSize);
}

void Game::Begin()
{
	m_showMenu = false;

	// Tear down existing game
	if (m_world != nullptr || m_scene != nullptr)
	{
		assert(m_world != nullptr);
		assert(m_scene != nullptr);
		m_world.release();
		m_scene.release();
	}

	// Initialize Box2D world
	m_world = std::unique_ptr<b2World>(new b2World(b2Vec2(0,0)));
	m_scene = std::unique_ptr<Scene>(new Scene(m_size,
		m_ctx,
		m_world.get(),
		m_dwriteFactory,
		this));
}

void Game::Resize(b2Vec2 viewportSize)
{
	if (m_scene != nullptr)
	{
		m_scene->Resize(viewportSize);
	}
	if (m_menu != nullptr)
	{
		m_menu->Resize(viewportSize);
	}
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

void Game::OnMouseDown(b2Vec2 p, bool left, bool right)
{
	if (right)
	{
		m_showMenu = !m_showMenu;
	}
	else if (left)
	{
		if (m_showMenu)
		{
			assert(m_menu);
			m_menu->OnMouseDown(p);
		}
		else
		{
			assert(m_scene);
			m_scene->OnMouseDown(p);
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