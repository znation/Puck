//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

#include <initguid.h>
#include <d3d11_1.h>
#include <dxgi1_2.h>
#include <d2d1effects.h>
#include <dwrite_1.h>
#include <wincodec.h>
#include "Utility.h"

ref class DirectXBase abstract
{
public:
    DirectXBase();

#ifdef WINRT
    virtual void Initialize(Windows::UI::Core::CoreWindow^ window, float dpi);
#else
	virtual void Initialize(float dpi);
#endif
    virtual void CreateDeviceIndependentResources();
    virtual void CreateDeviceResources();
    virtual void CreateWindowSizeDependentResources();
    virtual void UpdateForWindowSizeChange();
    virtual void SetDpi(float dpi);
    virtual void Render() = 0;
    virtual void Present();

protected:

#ifdef WINRT
    Windows::UI::Core::CoreWindow^                  m_window;
	Windows::Foundation::Size                       m_renderTargetSize;
    Windows::Foundation::Rect                       m_windowBounds;
#endif

    // Declare Direct2D Objects
#ifdef WINRT
    ComPtr<ID2D1Factory1>           m_d2dFactory;
	ComPtr<ID2D1Device>             m_d2dDevice;
    ComPtr<ID2D1DeviceContext>      m_d2dDeviceContext;
	ComPtr<ID2D1Bitmap1>            m_d2dTargetBitmap;
#else
	ComPtr<ID2D1Factory>			m_d2dFactory;
	ComPtr<ID2D1Bitmap>				m_d2dTargetBitmap;
#endif

	ComPtr<ID2D1RenderTarget>		m_d2dContext;

    // Direct3D Objects
    ComPtr<ID3D11Device1>           m_d3dDevice;
    ComPtr<ID3D11DeviceContext1>    m_d3dContext;
    ComPtr<IDXGISwapChain1>         m_swapChain;
    ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

    D3D_FEATURE_LEVEL                               m_featureLevel;
    float                                           m_dpi;
};