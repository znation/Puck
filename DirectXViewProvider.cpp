//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#include "DirectXViewProvider.h"
#include "View.h"

using namespace Windows::ApplicationModel::Core;
using namespace Windows::UI::Core;
using namespace Windows::ApplicationModel::Infrastructure;
using namespace Windows::ApplicationModel::Activation;

DirectXViewProvider::DirectXViewProvider()
{
}

void DirectXViewProvider::Initialize(
									 _In_ Windows::UI::Core::CoreWindow^ window,
									 _In_ Windows::ApplicationModel::Core::CoreApplicationView^ applicationView
									 )
{
	m_window = window;
	m_applicationView = applicationView;
}

// this method is called after Initialize
void DirectXViewProvider::Load(Platform::String^ entryPoint)
{
}

// this method is called after Load
void DirectXViewProvider::Run()
{
	auto view = ref new View();
	view->Initialize(m_window, m_applicationView);
	view->Run();

	// Must delete the view explicitly in order to break a circular dependency
	// between View and CoreWindow. View holds on to a CoreWindow reference most
	// typically for window activation, while CoreWindow refers back to View when
	// event handlers are hooked up. Without breaking this circular dependency,
	// neither View nor CoreWindow object gets to clean up. It's also important
	// to note that a 'delete' call on a ref class instance simply means calling
	// into a class destructor in order to explicitly break a cycle. It doesn't
	// actually deallocate any memory.
	delete view;
}

void DirectXViewProvider::Uninitialize()
{
}

IViewProvider^ DirectXViewProviderFactory::CreateViewProvider()
{
	return ref new DirectXViewProvider();
}
