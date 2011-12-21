//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

#include <wrl.h>

typedef enum class ActivationEntryPoint
{
    Unknown,
    DirectXApplication
} ActivationEntryPoint;

ref class DirectXViewProvider : public Windows::ApplicationModel::Infrastructure::IViewProvider
{
public:
    DirectXViewProvider();

    // IViewProvider Methods
    void Initialize(
        _In_ Windows::UI::Core::CoreWindow^ window,
        _In_ Windows::ApplicationModel::Core::CoreApplicationView^ applicationView
        );

    void Load(_In_ Platform::String^ entryPoint);
    void Run();
    void Uninitialize();

private:
    Windows::UI::Core::CoreWindow^ m_window;
    Windows::ApplicationModel::Core::CoreApplicationView^ m_applicationView;
};

ref class DirectXViewProviderFactory : Windows::ApplicationModel::Infrastructure::IViewProviderFactory 
{
public:
    Windows::ApplicationModel::Infrastructure::IViewProvider^ CreateViewProvider();
};
