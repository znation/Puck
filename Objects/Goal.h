#include "Box2D\Box2D.h"
#include "b2UserData.h"
#include "Utility.h"

#pragma once

// forward declaration of Scene (to prevent a circular .h dependency)
class Scene;

class Goal
{
public:
	Goal(b2Vec2 sceneSize,
		b2Vec2 scenePosition,
		int playerIdx,
		DeviceContext *ctx,
		b2World *world,
		Scene *scene);
	void draw();
	void detectCollisions();

private:
	DeviceContext *m_ctx;
	int m_playerIdx;
	Scene *m_scene;
	b2Vec2 m_size;
	b2Vec2 m_position;
	SolidColorBrush *m_brush;
	D2D1_RECT_F m_rect;
	b2Body *m_goalBody;
	std::unique_ptr<b2UserData> m_userData;
};
