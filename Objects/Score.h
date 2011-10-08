#include <wrl.h>
#include <d2d1_1.h>
#include "Box2D\Box2D.h"
#include <dwrite_1.h>

#pragma once

using namespace Microsoft::WRL;

class Score
{
public:
	Score(b2Vec2 playerSize, b2Vec2 playerPosition, int playerIdx, ComPtr<ID2D1DeviceContext> ctx, ComPtr<IDWriteFactory1> dwriteFactory);
	void draw();
	void increment();
private:
	int m_playerIdx;
	ComPtr<IDWriteTextFormat> m_format;
	ComPtr<ID2D1DeviceContext> m_ctx;
	void setScore(int score);
	int m_score;
	wchar_t m_scoreText[32];
	size_t m_scoreTextLength;
	D2D1_RECT_F m_layout;
	ComPtr<ID2D1SolidColorBrush> m_brush;
};