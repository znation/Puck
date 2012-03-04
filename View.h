//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

#include <wrl.h>
#include "D2DRenderer.h"
ref class View
{
public:
    View();
    
    void Initialize(
        _In_ Windows::UI::Core::CoreWindow^ window,
        _In_ Windows::ApplicationModel::Core::CoreApplicationView^ applicationView
        );

       // Declare Event Handlers
    void OnWindowSizeChanged(
        _In_ Windows::UI::Core::CoreWindow^ sender,
        _In_ Windows::UI::Core::WindowSizeChangedEventArgs^ args
        );

	void OnMouseMove(
		_In_ Windows::UI::Core::CoreWindow^ sender,
		_In_ Windows::UI::Core::PointerEventArgs^ args
        );

	void OnMouseDown(
		_In_ Windows::UI::Core::CoreWindow^ sender,
		_In_ Windows::UI::Core::PointerEventArgs^ args
        );

    void OnLogicalDpiChanged(__in Platform::Object^ sender);

    void Run();

private:

    D2DRenderer^											m_renderer;
    Windows::UI::Core::CoreWindow^                          m_window;
    Windows::ApplicationModel::Core::CoreApplicationView^   m_applicationView;
    float                                                   m_dpi;
};
