//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

#include <initguid.h>
#include <wincodec.h>
#include "Utility.h"

class DirectXBase abstract
{
public:
    DirectXBase();

#ifdef WINRT
    virtual void Initialize(Windows::UI::Core::CoreWindow^ window, float dpi);
	virtual void CreateDeviceResources();
#endif
    virtual void CreateDeviceIndependentResources();
    virtual void CreateWindowSizeDependentResources();
    virtual void UpdateForWindowSizeChange();
    virtual void SetDpi(float dpi);
    virtual void Render() = 0;
    virtual void Present();

protected:

#ifdef WINRT
    Windows::UI::Core::CoreWindow^                  m_window;
	Microsoft::WRL::ComPtr<ID2D1Device>             m_d2dDevice;
#endif

    // Declare Direct2D Objects
    D2DFactory *m_d2dFactory;
	DeviceContext *m_d2dContext;
	D2DBitmap *m_d2dTargetBitmap;

	// Declare DirectWrite & Windows Imaging Component Objects
	ImagingFactory *m_wicFactory;

#ifdef WINRT
    // Direct3D Objects
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;
    D3D_FEATURE_LEVEL                               m_featureLevel;
    Windows::Foundation::Size                       m_renderTargetSize;
    Windows::Foundation::Rect                       m_windowBounds;
#endif

    float                                           m_dpi;
};
