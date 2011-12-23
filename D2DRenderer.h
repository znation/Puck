//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

#include "DirectXBase.h"
#include "Box2D\Box2D.h"
#include "Objects\Game.h"
#include "Utility.h"

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

template<class Interface>
inline void
SafeRelease(
    Interface **ppInterfaceToRelease
    )
{
    if (*ppInterfaceToRelease != NULL)
    {
        (*ppInterfaceToRelease)->Release();

        (*ppInterfaceToRelease) = NULL;
    }
}

class D2DRenderer : public DirectXBase
{
public:
	D2DRenderer();
	~D2DRenderer();

	virtual void CreateDeviceIndependentResources();
	virtual void CreateDeviceResources();
	virtual void Render();
	void OnMouseMove(PointerEventArgs *args);
	void OnMouseDown(PointerEventArgs *args);
#ifndef WINRT
	HRESULT Initialize();
	void RunMessageLoop();
	HRESULT OnRender();
    void OnResize(
        UINT width,
        UINT height
        );
    static LRESULT CALLBACK WndProc(
        HWND hWnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam
        );
#endif

private:
	void RecreateTarget();
	void RenderFPS(D2D1_SIZE_F);
	Game *m_game;
	int m_frameCounter;
	int m_fps;
	WORD m_lastFrameSecond;
	WriteFactory *m_dwriteFactory; // used for FPS text and game menu
#ifdef DEBUG
	SolidColorBrush *m_whiteBrush; // used only for FPS text
	IDWriteTextFormat *m_textFormat; // only used for FPS text
	WCHAR m_fpsText[10]; // used only for FPS text
	uint32 m_fpsTextLength; // used only for FPS text
#endif
#ifndef WINRT
	HWND m_hwnd;
#endif
};
