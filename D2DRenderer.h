//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

#include <wrl.h>

#include "DirectXBase.h"
#include "Box2D\Box2D.h"
#include "Objects\Scene.h"

using namespace Microsoft::WRL;

double pixelToBox(int coord);
int boxToPixel(double coord);

ref class D2DRenderer : public DirectXBase
{
public:
	D2DRenderer();

	virtual void CreateDeviceIndependentResources() override;
	virtual void CreateDeviceResources() override;
	virtual void Render() override;
	void OnMouseMoved(Windows::UI::Core::PointerEventArgs^ args);

private:
	void RecreateTarget();
	void RenderFPS();
	Scene *scene;
	b2World *world;
	int m_frameCounter;
	int m_fps;
	WORD m_lastFrameSecond;

	ComPtr<ID2D1SolidColorBrush> m_whiteBrush; // used only for FPS text
	ComPtr<IDWriteFactory1> m_dwriteFactory; // only used for FPS text
	ComPtr<IDWriteTextFormat> m_textFormat; // only used for FPS text
	WCHAR m_fpsText[10]; // used only for FPS text
	uint32 m_fpsTextLength; // used only for FPS text
};
