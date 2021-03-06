//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#include <math.h>
#include "D2DRenderer.h"
#include "windows.h"
#include "Utility.h"

using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;

D2DRenderer::D2DRenderer()
{
	m_d2dContext = nullptr;
	m_game = nullptr;

	ThrowIfFailed(
		DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		(IUnknown **)(&m_dwriteFactory)
		)
		);

#ifdef DEBUG
	ThrowIfFailed(
		m_dwriteFactory->CreateTextFormat(
		L"Segoe UI",
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		32.0f,
		L"en-US",
		&m_textFormat));
#endif
}

D2DRenderer::~D2DRenderer()
{
	assert(m_game != NULL);
	delete m_game;
}

void D2DRenderer::CreateDeviceIndependentResources()
{
	DirectXBase::CreateDeviceIndependentResources();
}

void D2DRenderer::CreateDeviceResources()
{
	DirectXBase::CreateDeviceResources();
}

void D2DRenderer::RecreateTarget()
{
	m_d2dContext->SetTarget(nullptr);
	m_d2dTargetBitmap = nullptr;
	CreateDeviceResources();
	CreateWindowSizeDependentResources();
}

#ifdef DEBUG
void D2DRenderer::RenderFPS(D2D1_SIZE_F)
{
	SYSTEMTIME time;
	GetSystemTime(&time);
	if (time.wSecond != m_lastFrameSecond)
	{
		m_lastFrameSecond = time.wSecond;
		m_fps = m_frameCounter;
		m_frameCounter = 0;

		char buf[20];
		memset(buf, 0, 20);
		sprintf_s(buf, 20, "FPS: %d", m_fps);
		mbstowcs_s(&m_fpsTextLength, m_fpsText, strlen(buf)+1, buf, _TRUNCATE);
	}
	else
	{
		m_frameCounter++;
	}

	ThrowIfFailed(
		m_d2dContext->CreateSolidColorBrush(D2D1::ColorF(1.0, 1.0, 1.0, 0.5), &m_whiteBrush)
		);

	D2D1_RECT_F layoutRect;
	layoutRect.top = 150;
	layoutRect.left = 60;
	layoutRect.bottom = 400;
	layoutRect.right = 400;

	m_d2dContext->DrawText(
		m_fpsText,
		m_fpsTextLength,
		m_textFormat,
		&layoutRect,
		m_whiteBrush);
}
#endif

void D2DRenderer::OnMouseMove(PointerEventArgs ^args)
{
	Point p = args->CurrentPoint->Position;
	m_game->OnMouseMove(b2Vec2(p.X, p.Y));
}

void D2DRenderer::OnMouseDown(PointerEventArgs ^args)
{
	m_game->OnMouseDown(args);
}

void D2DRenderer::Render()
{
	HRESULT hr = S_OK;

	// Retrieve the size of the render target.
	D2D1_SIZE_F renderTargetSize = m_d2dContext->GetSize();

	if (m_game == nullptr)
	{
		// Initialize Game
		m_game = new Game(b2Vec2(renderTargetSize.width, renderTargetSize.height), m_d2dContext, m_dwriteFactory);
	}

	m_d2dContext->BeginDraw();
	m_d2dContext->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	m_d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());

	m_game->Draw();

#ifdef DEBUG
	RenderFPS(renderTargetSize);
#endif

	hr = m_d2dContext->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		RecreateTarget();
	}
	else
	{
		// TODO why is it failing here?
		//ThrowIfFailed(hr);
	}

	Present();
}
