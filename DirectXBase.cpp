//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

// DirectXBase is a helper class that demonstrates initializing the Direct3D and Direct2D APIs in samples
#include "DirectXBase.h" 

using namespace Windows::UI::Core;
using namespace Windows::Foundation;
using namespace Microsoft::WRL;
using namespace D2D1;

// Constructor.
DirectXBase::DirectXBase()
{
}

// Initialize the Direct3D resources required to run.
void DirectXBase::Initialize(CoreWindow^ window, float dpi)
{
    m_window = window;
    m_dpi = dpi;

    CreateDeviceIndependentResources();
    CreateDeviceResources();
    CreateWindowSizeDependentResources();
}

// These are the resources required independent of hardware.
void DirectXBase::CreateDeviceIndependentResources()
{
    D2D1_FACTORY_OPTIONS options;
    ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
     // If the project is in a debug build, enable Direct2D debugging via SDK Layers
    options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

    ThrowIfFailed(
        D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED, 
            __uuidof(ID2D1Factory1),
            &options, 
            (LPVOID *) &m_d2dFactory
            )
        );

    ThrowIfFailed(
        CoCreateInstance(
            CLSID_WICImagingFactory,
            nullptr,
            CLSCTX_INPROC_SERVER,
            __uuidof(IWICImagingFactory2),
            (LPVOID *) &m_wicFactory
            )
        );

}

// These are the resources that depend on the device,
// and so will need to be re-allocated if it is rebuilt (such as on a SizeChanged event).
void DirectXBase::CreateDeviceResources()
{
    // This flag adds support for surfaces with a different color channel ordering than the default.
    // It is required for compatibility with Direct2D.
    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
    ComPtr<IDXGIDevice> dxgiDevice;

#ifdef DEBUG
    // If the project is in a debug build, enable debugging via SDK Layers with this flag.
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // This array defines the set of DirectX hardware feature levels this app will support.
    // Note the ordering should be preserved.
    D3D_FEATURE_LEVEL featureLevels[] = 
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
// Debug layers support for feature level 9 currently not available; will be enabled in a future release.
#if !defined(_DEBUG)
		D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
#endif
    };

	// Create the DX11 API device object, and get a corresponding context.
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    ThrowIfFailed(
        D3D11CreateDevice(
            nullptr,                    // specify null to use the default adapter
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,                    // leave as null if hardware is used
            creationFlags,              // optionally set debug and Direct2D compatibility flags
            featureLevels,
            ARRAYSIZE(featureLevels),
            D3D11_SDK_VERSION,          // always set this to D3D11_SDK_VERSION
            &device,
            &m_featureLevel,
            &context
            )
        );

    // Get the DirectX11.1 device by QI off the DirectX11 one.
    ThrowIfFailed(
        device.As(&m_d3dDevice)
        );

    // And get the corresponding device context in the same way.
    ThrowIfFailed(
        context.As(&m_d3dContext)
        );

    // Obtain the underlying DXGI device of the Direct3D11.1 device.
    ThrowIfFailed(
        m_d3dDevice.As(&dxgiDevice)
        );

    // Obtain the Direct2D device for 2-D rendering.
    ThrowIfFailed(
        m_d2dFactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice)
        );

    // And get its corresponding device context object.
    ThrowIfFailed(
        m_d2dDevice->CreateDeviceContext(
            D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
            &m_d2dContext
        )
        );

    // Save the DPI of this display in our class.
    m_d2dContext->SetDpi(m_dpi, m_dpi);

    // Release the swap chain (if it exists) as it will be incompatible with
    // the new device.
    m_swapChain = nullptr;
}

// Allocate all memory resources that change on a window SizeChanged event.
void DirectXBase::CreateWindowSizeDependentResources()
{
    // Store the window bounds so the next time we get a SizeChanged event we can
    // avoid rebuilding everything if the size is identical.
    m_windowBounds = m_window->Bounds;

    // If the swap chain already exists, resize it.
    if(m_swapChain != nullptr)
    {
        ThrowIfFailed(
            m_swapChain->ResizeBuffers(2, 0, 0, DXGI_FORMAT_B8G8R8A8_UNORM, 0)
            );
    }
    // Otherwise, create a new one.
    else
    {
        // Allocate a descriptor.
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};
        swapChainDesc.Width = 0;                                     // use automatic sizing
        swapChainDesc.Height = 0;
        swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;           // this is the most common swapchain format
        swapChainDesc.Stereo = false; 
        swapChainDesc.SampleDesc.Count = 1;                          // don't use multi-sampling
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 2;                               // use two buffers to enable flip effect
        swapChainDesc.Scaling = DXGI_SCALING_NONE;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // we recommend using this swap effect for all applications
        swapChainDesc.Flags = 0;

        // Once the desired swap chain description is configured, it must be created on the same adapter as our D3D Device

        // First, retrieve the underlying DXGI Device from the D3D Device.
        ComPtr<IDXGIDevice1>  dxgiDevice;
        ThrowIfFailed(
            m_d3dDevice.As(&dxgiDevice)
            );

        // Ensure that DXGI does not queue more than one frame at a time. This both reduces 
        // latency and ensures that the application will only render after each VSync, minimizing 
        // power consumption.
        ThrowIfFailed(
            dxgiDevice->SetMaximumFrameLatency(1)
            );

        // Identify the physical adapter (GPU or card) this device is running on.
        ComPtr<IDXGIAdapter> dxgiAdapter;
        ThrowIfFailed(
            dxgiDevice->GetAdapter(&dxgiAdapter)
            );

        // And obtain the factory object that created it.
        ComPtr<IDXGIFactory2> dxgiFactory;
        ThrowIfFailed(
            dxgiAdapter->GetParent(
                __uuidof(IDXGIFactory2), 
                &dxgiFactory
                )
            );

        // Obtain the final swap chain for this window from the DXGI factory.
        ThrowIfFailed(
            dxgiFactory->CreateSwapChainForCoreWindow(
                m_d3dDevice.Get(),
                GetIUnknown(m_window),
                &swapChainDesc,
                nullptr,    // allow on all displays
                &m_swapChain
                )
            );
    }

    // Obtain the backbuffer for this window which will be the final 3D rendertarget.
    ComPtr<ID3D11Texture2D> backBuffer;
    ThrowIfFailed(
        m_swapChain->GetBuffer(
            0,
            __uuidof(ID3D11Texture2D),
            &backBuffer
            )
        );

    // Create a view interface on the rendertarget to use on bind.
    ThrowIfFailed(
        m_d3dDevice->CreateRenderTargetView(
            backBuffer.Get(),
            nullptr,
            &m_renderTargetView
            )
        );

    // Cache the rendertarget dimensions in our helper class for convenient use.
    D3D11_TEXTURE2D_DESC backBufferDesc = {0};
    backBuffer->GetDesc(&backBufferDesc);
    m_renderTargetSize.Width  = static_cast<float>(backBufferDesc.Width);
    m_renderTargetSize.Height = static_cast<float>(backBufferDesc.Height);

    // Create a descriptor for the depth/stencil buffer.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(
        DXGI_FORMAT_D24_UNORM_S8_UINT, 
        backBufferDesc.Width,
        backBufferDesc.Height,
        1,
        1,
        D3D11_BIND_DEPTH_STENCIL);

    // Allocate a 2-D surface as the depth/stencil buffer.
    ComPtr<ID3D11Texture2D> depthStencil;
    ThrowIfFailed(
        m_d3dDevice->CreateTexture2D(
            &depthStencilDesc,
            nullptr,
            &depthStencil
            )
        );

    // Create a DepthStencil view on this surface to use on bind.
	CD3D11_DEPTH_STENCIL_VIEW_DESC desc = CD3D11_DEPTH_STENCIL_VIEW_DESC(D3D11_DSV_DIMENSION_TEXTURE2D);
    ThrowIfFailed(
        m_d3dDevice->CreateDepthStencilView(
            depthStencil.Get(),
            &desc,
            &m_depthStencilView
            )
        );

    // Create a viewport descriptor of the full window size.
    CD3D11_VIEWPORT viewport(
        0.0f,
        0.0f,
        static_cast<float>(backBufferDesc.Width),
        static_cast<float>(backBufferDesc.Height)
        );

    // Set the current viewport using the descriptor.
    m_d3dContext->RSSetViewports(1, &viewport);

    // Now we set up the Direct2D render target bitmap linked to the swapchain. 
    // Whenever we render to this bitmap, it will be directly rendered to the 
    // swapchain associated with the window.
    D2D1_BITMAP_PROPERTIES1 bitmapProperties = 
        BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
            PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
            m_dpi,
            m_dpi
            );

    // Direct2D needs the dxgi version of the backbuffer surface pointer.
    ComPtr<IDXGISurface> dxgiBackBuffer;
    ThrowIfFailed(
        m_swapChain->GetBuffer(
            0,
            __uuidof(IDXGISurface),
            &dxgiBackBuffer
            )
        );

    // Get a D2D surface from the DXGI back buffer to use as the D2D render target.
    ThrowIfFailed(
        m_d2dContext->CreateBitmapFromDxgiSurface(
            dxgiBackBuffer.Get(),
            &bitmapProperties,
            &m_d2dTargetBitmap
            )
        );

    // So now we can set the Direct2D render target.
    m_d2dContext->SetTarget(m_d2dTargetBitmap);

    // Set D2D text anti-alias mode to Grayscale to ensure proper rendering of text on intermediate surfaces.
    m_d2dContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
}

// This routine is called in the event handler for the view SizeChanged event.
void DirectXBase::UpdateForWindowSizeChange()
{
    if (m_window->Bounds.Width  != m_windowBounds.Width ||
        m_window->Bounds.Height != m_windowBounds.Height)
    {
        m_d2dContext->SetTarget(nullptr);
        m_d2dTargetBitmap = nullptr;
        m_renderTargetView = nullptr;
        m_depthStencilView = nullptr;
        CreateWindowSizeDependentResources();
    }
}

// Helps track the DPI in the helper class.
// This is called in the dpiChanged event handler in the view class.
void DirectXBase::SetDpi(float dpi)
{
    if (dpi != m_dpi)
    {
        m_dpi = dpi;

        // Often a DPI change implies a window size change. In some cases Windows will issues
        // both a size changed event and a DPI changed event. In this case, the resulting bounds 
        // will not change, and the window resize code will only be executed once.
        UpdateForWindowSizeChange();
    }
}

// Method to deliver the final image to the display.
void DirectXBase::Present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present(1, 0);

    // If the device was removed either by a disconnect or a driver upgrade, we 
    // must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        Initialize(m_window, m_dpi);
    }
    else
    {
        ThrowIfFailed(hr);
    }
}
