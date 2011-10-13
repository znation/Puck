#include "TopBar.h"
#include "Scene.h"

TopBar::TopBar(Scene *scene,
			   ComPtr<ID2D1DeviceContext> ctx,
			   b2Vec2 topBarSize,
			   b2Vec2 topBarPosition)
{
	m_scene = scene;
	m_ctx = ctx;
	m_size = topBarSize;
	m_position = topBarPosition;

	b2Vec2 buttonAreaSize = b2Vec2(300, topBarSize.y);
	b2Vec2 buttonAreaPosition = b2Vec2(m_position.x + (m_size.x / 2.0) - (buttonAreaSize.x / 2.0),
		topBarPosition.y);

	for (int i=0; i<2; i++)
	{
		b2Vec2 scoreSize = b2Vec2((topBarSize.x - buttonAreaSize.x) / 2.0, topBarSize.y);
		b2Vec2 scorePosition = b2Vec2(i == 0 ?
			topBarPosition.x :
		topBarPosition.x + scoreSize.x + buttonAreaSize.x,
			topBarPosition.y);
		m_score[i] = new Score(scoreSize, scorePosition, i, ctx);
	}
}

void TopBar::Draw()
{
	for (int i=0; i<2; i++)
	{
		m_score[i]->draw();
	}
}

void TopBar::ScoreGoal(int playerIdx)
{
	m_score[playerIdx]->increment();
	int score = m_score[playerIdx]->getScore();
	if (score == MAX_SCORE)
	{
		m_scene->win(playerIdx);
	}
	else
	{
		m_scene->reset();
	}
}

