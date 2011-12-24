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

#ifdef WINRT
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
#endif

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
#ifdef WINRT
	DirectXBase::CreateDeviceIndependentResources();
#else
    // Create a Direct2D factory.
    ThrowIfFailed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_d2dFactory));
#endif
}

void D2DRenderer::CreateDeviceResources()
{
#ifdef WINRT
	DirectXBase::CreateDeviceResources();
#else
    if (!m_d2dContext)
    {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(
            rc.right - rc.left,
            rc.bottom - rc.top
            );

        // Create a Direct2D render target.
        ThrowIfFailed(m_d2dFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hwnd, size),
            &m_d2dContext
            ));
	}
#endif
}

void D2DRenderer::RecreateTarget()
{
#ifdef WINRT
	m_d2dContext->SetTarget(nullptr);
#else
	SafeRelease(&m_d2dContext);
#endif

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

void D2DRenderer::OnMouseMove(PointerEventArgs *args)
{
	Position p = args->CurrentPoint->Position;
	m_game->OnMouseMove(b2Vec2(p.X, p.Y));
}

void D2DRenderer::OnMouseDown(PointerEventArgs *args)
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

#ifndef WINRT

HRESULT D2DRenderer::Initialize()
{
	HRESULT hr;

    // Initialize device-indpendent resources, such
    // as the Direct2D factory.
    CreateDeviceIndependentResources();

    // Register the window class.
    WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = D2DRenderer::WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = sizeof(LONG_PTR);
    wcex.hInstance     = HINST_THISCOMPONENT;
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName  = NULL;
    wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcex.lpszClassName = L"Puck";

    RegisterClassEx(&wcex);

    // Create the application window.
    //
    // Because the CreateWindow function takes its size in pixels, we
    // obtain the system DPI and use it to scale the window size.
    FLOAT dpiX, dpiY;
    m_d2dFactory->GetDesktopDpi(&dpiX, &dpiY);

    m_hwnd = CreateWindow(
        L"Puck",
        L"Puck",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        static_cast<UINT>(ceil(640.f * dpiX / 96.f)),
        static_cast<UINT>(ceil(480.f * dpiY / 96.f)),
        NULL,
        NULL,
        HINST_THISCOMPONENT,
        this
        );

    hr = m_hwnd ? S_OK : E_FAIL;
    if (SUCCEEDED(hr))
    {
        ShowWindow(m_hwnd, SW_SHOWNORMAL);

        UpdateWindow(m_hwnd);
    }

    return hr;
}

void D2DRenderer::RunMessageLoop()
{
	MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

//
// The window message handler.
//
LRESULT CALLBACK D2DRenderer::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    if (message == WM_CREATE)
    {
        LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
        D2DRenderer *pD2DRenderer = (D2DRenderer *)pcs->lpCreateParams;

        ::SetWindowLongPtrW(
            hwnd,
            GWLP_USERDATA,
            PtrToUlong(pD2DRenderer)
            );

		SetTimer(hwnd, 1, (UINT)(1000.0f / FRAMERATE), NULL);

        result = 1;
    }
    else
    {
        D2DRenderer *pD2DRenderer = reinterpret_cast<D2DRenderer *>(static_cast<LONG_PTR>(
            ::GetWindowLongPtrW(
                hwnd,
                GWLP_USERDATA
                )));

        bool wasHandled = false;

        if (pD2DRenderer)
        {
            switch (message)
            {
			case WM_MOUSEMOVE:
				{
					// TODO Win7 mouse event handling (see MSDN for examples)
				}

            case WM_SIZE:
                {
                    UINT width = LOWORD(lParam);
                    UINT height = HIWORD(lParam);
                    pD2DRenderer->OnResize(width, height);
                }
                wasHandled = true;
                result = 0;
                break;

            case WM_DISPLAYCHANGE:
                {
                    InvalidateRect(hwnd, NULL, FALSE);
                }
                wasHandled = true;
                result = 0;
                break;

            case WM_PAINT:
			case WM_TIMER:
                {
                    pD2DRenderer->OnRender();

                    ValidateRect(hwnd, NULL);
                }
                wasHandled = true;
                result = 0;
                break;

            case WM_DESTROY:
                {
                    PostQuitMessage(0);
                }
                wasHandled = true;
                result = 1;
                break;
            }
        }

        if (!wasHandled)
        {
            result = DefWindowProc(hwnd, message, wParam, lParam);
        }
    }

    return result;
}

//  Called whenever the application needs to display the client
//  window. This method writes "Hello, World"
//
//  Note that this function will automatically discard device-specific
//  resources if the Direct3D device disappears during function
//  invocation, and will recreate the resources the next time it's
//  invoked.
//
HRESULT D2DRenderer::OnRender()
{
    CreateDeviceResources();
	Render();
	return S_OK;
}

//
//  If the application receives a WM_SIZE message, this method
//  resizes the render target appropriately.
//
void D2DRenderer::OnResize(UINT width, UINT height)
{
    if (m_d2dContext)
    {
        D2D1_SIZE_U size;
        size.width = width;
        size.height = height;

        // Note: This method can fail, but it's okay to ignore the
        // error here -- it will be repeated on the next call to
        // EndDraw.
		// TODO what do I call here? Resize is not defined
        //m_d2dContext->Resize(size);
    }
}

#endif
