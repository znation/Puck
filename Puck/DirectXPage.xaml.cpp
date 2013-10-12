//
// DirectXPage.xaml.cpp
// Implementation of the DirectXPage.xaml class.
//

#include "pch.h"
#include "DirectXPage.xaml.h"

using namespace Puck;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::Input;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

DirectXPage::DirectXPage()
{
	InitializeComponent();

	m_renderer = ref new SimpleTextRenderer();

	m_renderer->Initialize(
		Window::Current->CoreWindow,
		SwapChainPanel,
		DisplayProperties::LogicalDpi
		);

	Window::Current->CoreWindow->SizeChanged += 
		ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &DirectXPage::OnWindowSizeChanged);

	DisplayProperties::LogicalDpiChanged +=
		ref new DisplayPropertiesEventHandler(this, &DirectXPage::OnLogicalDpiChanged);

	DisplayProperties::OrientationChanged +=
		ref new DisplayPropertiesEventHandler(this, &DirectXPage::OnOrientationChanged);

	DisplayProperties::DisplayContentsInvalidated +=
		ref new DisplayPropertiesEventHandler(this, &DirectXPage::OnDisplayContentsInvalidated);

	m_eventToken = CompositionTarget::Rendering::add(ref new EventHandler<Object^>(this, &DirectXPage::OnRendering));

	m_timer = ref new BasicTimer();
}


void DirectXPage::OnPointerMoved(Object^ sender, PointerRoutedEventArgs^ args)
{
	PointerPoint ^p = args->GetCurrentPoint(nullptr);
	m_renderer->OnMouseMove(p->Position);
}

void DirectXPage::OnPointerPressed(Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ args)
{
	PointerPoint ^p = args->GetCurrentPoint(nullptr);
	bool left = p->Properties->IsLeftButtonPressed;
	bool right = p->Properties->IsRightButtonPressed;
	m_renderer->OnMouseDown(p->Position, left, right);
}

void DirectXPage::OnPointerReleased(Object^ sender, PointerRoutedEventArgs^ args)
{
	// TODO -- what should happen on release?
}

void DirectXPage::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
	m_renderer->UpdateForWindowSizeChange();
}

void DirectXPage::OnLogicalDpiChanged(Object^ sender)
{
	m_renderer->SetDpi(DisplayProperties::LogicalDpi);
}

void DirectXPage::OnOrientationChanged(Object^ sender)
{
	m_renderer->UpdateForWindowSizeChange();
}

void DirectXPage::OnDisplayContentsInvalidated(Object^ sender)
{
	m_renderer->ValidateDevice();
}

void DirectXPage::OnRendering(Object^ sender, Object^ args)
{
	m_timer->Update();
	m_renderer->Update(m_timer->Total, m_timer->Delta);
	m_renderer->Render();
	m_renderer->Present();
}

void DirectXPage::OnPreviousColorPressed(Object^ sender, RoutedEventArgs^ args)
{
}

void DirectXPage::OnNextColorPressed(Object^ sender, RoutedEventArgs^ args)
{
}

void DirectXPage::SaveInternalState(IPropertySet^ state)
{
	m_renderer->SaveInternalState(state);
}

void DirectXPage::LoadInternalState(IPropertySet^ state)
{
	m_renderer->LoadInternalState(state);
}
