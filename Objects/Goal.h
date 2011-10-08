#include <wrl.h>
#include <d2d1_1.h>
#include "Box2D\Box2D.h"
#include "Score.h"

#pragma once

using namespace Microsoft::WRL;

// forward declaration of Scene (to prevent a circular .h dependency)
class Scene;

class Goal
{
public:
	Goal(b2Vec2 sceneSize, b2Vec2 scenePosition, int playerIdx, ComPtr<ID2D1DeviceContext> ctx, b2World *world, Scene *scene, Score *score);
	void draw();
	void detectCollisions();

private:
	ComPtr<ID2D1DeviceContext> m_ctx;
	int m_playerIdx;
	Score *m_score;
	Scene *m_scene;
	b2Vec2 m_size;
	b2Vec2 m_position;
	ComPtr<ID2D1SolidColorBrush> m_brush;
	D2D1_RECT_F m_rect;
	b2Body *m_goalBody;
};
