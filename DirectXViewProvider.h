//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

#include "Utility.h"

typedef enum class ActivationEntryPoint
{
    Unknown,
    DirectXApplication
} ActivationEntryPoint;

ref class DirectXViewProvider
#ifdef WINRT
	: public Windows::ApplicationModel::Infrastructure::IViewProvider
#endif
{
public:
    DirectXViewProvider();

    // IViewProvider Methods
#ifdef WINRT
    void Initialize(
        _In_ Windows::UI::Core::CoreWindow^ window,
        _In_ Windows::ApplicationModel::Core::CoreApplicationView^ applicationView
        );

    void Load(_In_ Platform::String^ entryPoint);
#endif
    void Run();
    void Uninitialize();

private:
#ifdef WINRT
    Windows::UI::Core::CoreWindow^ m_window;
    Windows::ApplicationModel::Core::CoreApplicationView^ m_applicationView;
#endif
};

ref class DirectXViewProviderFactory
#ifdef WINRT
	: Windows::ApplicationModel::Infrastructure::IViewProviderFactory
#endif
{
public:
#ifdef WINRT
    Windows::ApplicationModel::Infrastructure::IViewProvider^ CreateViewProvider();
#endif
};
