//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

#include "Utility.h"
#include "DirectXBase.h"
#include "Box2D\Box2D.h"
#include "Objects\Game.h"

ref class D2DRenderer : public DirectXBase
{
public:
	D2DRenderer();
	~D2DRenderer();

	virtual void CreateDeviceIndependentResources() override;
	virtual void CreateDeviceResources() override;
	virtual void Render() override;
#ifdef WINRT
	void OnMouseMove(Windows::UI::Core::PointerEventArgs^ args);
	void OnMouseDown(Windows::UI::Core::PointerEventArgs^ args);
#endif

private:
	void RecreateTarget();
	void RenderFPS(D2D1_SIZE_F);
	Game *m_game;
	int m_frameCounter;
	int m_fps;
	WORD m_lastFrameSecond;
	ComPtr<IDWriteFactory1> m_dwriteFactory; // used for FPS text and game menu
#ifdef DEBUG
	ComPtr<ID2D1SolidColorBrush> m_whiteBrush; // used only for FPS text
	ComPtr<IDWriteTextFormat> m_textFormat; // only used for FPS text
	WCHAR m_fpsText[10]; // used only for FPS text
	uint32 m_fpsTextLength; // used only for FPS text
#endif
};
