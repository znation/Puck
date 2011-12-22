//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#include "View.h"
#include "ThemeMusic.h"

#ifdef WINRT

using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;

View::View() :
    m_dpi(96.0f)
{
}

void View::Initialize(
    _In_ CoreWindow^ window,
    _In_ CoreApplicationView^ applicationView
    )
{
    m_window = window;
    m_applicationView = applicationView;

    m_dpi = static_cast<float>(DisplayProperties::LogicalDpi);

	m_window->PointerCursor = ref new CoreCursor(CoreCursorType::Cross, 0);

	m_window->PointerMoved += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &View::OnMouseMove);

	m_window->PointerPressed += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &View::OnMouseDown);

    m_window->SizeChanged += 
        ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &View::OnWindowSizeChanged);

    DisplayProperties::LogicalDpiChanged += 
        ref new DisplayPropertiesEventHandler(this, &View::OnLogicalDpiChanged);

    m_renderer = ref new D2DRenderer();
    m_renderer->Initialize(m_window, m_dpi);

	ThemeMusic *m = new ThemeMusic(window);
	m->Play();
}

void View::Run()
{
    m_window->Activate();

// Suppress warning C4127 (conditional expression is constant)
#pragma warning ( suppress : 4127 )
    while (true)
    {
        m_window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
        m_renderer->Render(); // this call is sychronized to the display frame rate
    }
}

void View::OnWindowSizeChanged(
    _In_ Windows::UI::Core::CoreWindow^,
    _In_ Windows::UI::Core::WindowSizeChangedEventArgs^
    )
{
    m_renderer->UpdateForWindowSizeChange();
}

void View::OnMouseMove(
		_In_ Windows::UI::Core::CoreWindow^,
		_In_ Windows::UI::Core::PointerEventArgs^ args
        )
{
	m_renderer->OnMouseMove(args);
}

void View::OnMouseDown(
		_In_ Windows::UI::Core::CoreWindow^,
		_In_ Windows::UI::Core::PointerEventArgs^ args
        )
{
	m_renderer->OnMouseDown(args);
}

void View::OnLogicalDpiChanged(__in Platform::Object^)
{
    m_dpi = static_cast<float>(DisplayProperties::LogicalDpi);
    m_renderer->SetDpi(m_dpi);
}

#endif