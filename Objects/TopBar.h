#include <wrl.h>
#include <d2d1_1.h>
#include "Box2D\Box2D.h"
#include <dwrite_1.h>
#include "Score.h"
#include "Utility.h"

#pragma once

using namespace Microsoft::WRL;

class Scene;

class TopBar
{
public:
	TopBar(Scene *scene,
		ComPtr<ID2D1DeviceContext> ctx,
		b2Vec2 topBarSize,
		b2Vec2 topBarPosition);
	void ScoreGoal(int playerIdx);
	void Draw();
private:
	ComPtr<ID2D1DeviceContext> m_ctx;
	Score *m_score[2];
	Scene *m_scene;
	b2Vec2 m_size;
	b2Vec2 m_position;
};
