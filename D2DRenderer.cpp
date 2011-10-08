//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#include <math.h>
#include "D2DRenderer.h"
#include <d2d1_1helper.h>
#include "windows.h"

using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;

double scalingFactor = 200.0;
double frameRate = 60.0;

int boxToPixel(double coord)
{
	return (int)(coord * scalingFactor);
}

double pixelToBox(int coord)
{
	return ((double)coord) / scalingFactor;
}

D2DRenderer::D2DRenderer()
{
	DX::ThrowIfFailed(
		DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		&m_dwriteFactory
		)
		);

	DX::ThrowIfFailed(
		m_dwriteFactory->CreateTextFormat(
		L"Segoe UI",
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		64.0f,
		L"en-US",
		&m_textFormat));
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

void D2DRenderer::RenderFPS(D2D1_SIZE_F)
{
	SYSTEMTIME time;
	GetSystemTime(&time);
	if (time.wSecond != m_lastFrameSecond)
	{
		m_lastFrameSecond = time.wSecond;
		m_fps = m_frameCounter;
		m_frameCounter = 0;

		char buf[10];
		sprintf(buf, "FPS: %d", m_fps);
		mbstowcs_s(&m_fpsTextLength, m_fpsText, strlen(buf)+1, buf, _TRUNCATE);
	}
	else
	{
		m_frameCounter++;
	}

	DX::ThrowIfFailed(
		m_d2dContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_whiteBrush)
		);

	D2D1_RECT_F layoutRect;
	layoutRect.top = 60;
	layoutRect.left = 60;
	layoutRect.bottom = 250;
	layoutRect.right = 400;

	m_d2dContext->DrawText(
		m_fpsText,
		m_fpsTextLength,
		m_textFormat.Get(),
		&layoutRect,
		m_whiteBrush.Get());
}

void D2DRenderer::OnMouseMoved(Windows::UI::Core::PointerEventArgs^ args)
{
	Point p = args->CurrentPoint->Position;
	scene->onMouseMoved(b2Vec2(p.X, p.Y));
}

void D2DRenderer::Render()
{
	HRESULT hr = S_OK;

	// Retrieve the size of the render target.
	D2D1_SIZE_F renderTargetSize = m_d2dContext->GetSize();

	if (scene == nullptr)
	{
		// Initialize Box2D world
		world = new b2World(b2Vec2(0,0));

		// Initialize scene
		scene = new Scene(b2Vec2(renderTargetSize.width, renderTargetSize.height), m_d2dContext, world, m_dwriteFactory);
	}

	m_d2dContext->BeginDraw();
	m_d2dContext->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	m_d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());

	scene->applyConstraints();
	world->Step(1.0 / 60.0, 8, 3);
	scene->detectCollisions();
	scene->move();
	scene->draw(m_d2dContext);

	RenderFPS(renderTargetSize);

	hr = m_d2dContext->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		RecreateTarget();
	}
	else
	{
		DX::ThrowIfFailed(hr);
	}

	Present();
}
