//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

#include <wrl.h>
#include <Windows.ApplicationModel.h>

typedef enum class ActivationEntryPoint
{
    Unknown,
    DirectXApplication
} ActivationEntryPoint;

ref class DirectXViewProvider : public Windows::ApplicationModel::Core::IFrameworkView
{
public:
    DirectXViewProvider();

    // IViewProvider Methods
    virtual void Initialize(
        _In_ Windows::ApplicationModel::Core::CoreApplicationView^ applicationView
    );

	virtual void SetWindow(
        _In_ Windows::UI::Core::CoreWindow^ window
	);

    virtual void Load(_In_ Platform::String^ entryPoint);
    virtual void Run();
    virtual void Uninitialize();

private:
    Windows::UI::Core::CoreWindow^ m_window;
    Windows::ApplicationModel::Core::CoreApplicationView^ m_applicationView;
};

ref class DirectXViewProviderFactory : public Windows::ApplicationModel::Core::IFrameworkViewSource
{
public:
	virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
};

