#include "pch.h"
#include "SimpleTextRenderer.h"

using namespace D2D1;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;

static const ColorF BackgroundColors[] = 
{
	ColorF::CornflowerBlue,
	ColorF::Orchid,
	ColorF::DeepPink,
	ColorF::Red,
	ColorF::DarkOrange,
	ColorF::Gold,
	ColorF::GreenYellow,
	ColorF::Aqua
};

SimpleTextRenderer::SimpleTextRenderer() :
	m_backgroundColorIndex(0),
	m_textPosition(0.0f, 0.0f)
{
}

void SimpleTextRenderer::CreateDeviceIndependentResources()
{
	DirectXBase::CreateDeviceIndependentResources();

#ifdef DEBUG
	DX::ThrowIfFailed(
		m_dwriteFactory->CreateTextFormat(
		L"Segoe UI",
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		32.0f,
		L"en-US",
		&m_textFormat));

	DX::ThrowIfFailed(
		m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING)
		);
#endif
}

void SimpleTextRenderer::CreateDeviceResources()
{
	DirectXBase::CreateDeviceResources();
}

void SimpleTextRenderer::CreateWindowSizeDependentResources()
{
	DirectXBase::CreateWindowSizeDependentResources();

	// Add code to create window size dependent objects here.
}

void SimpleTextRenderer::Update(float timeTotal, float timeDelta)
{
	(void) timeTotal; // Unused parameter.
	(void) timeDelta; // Unused parameter.

	// Add code to update time dependent objects here.
}

void SimpleTextRenderer::Render()
{
	/*
	m_d2dContext->BeginDraw();

	m_d2dContext->Clear(ColorF(BackgroundColors[m_backgroundColorIndex]));

	// Position the rendered text.
	Matrix3x2F translation = Matrix3x2F::Translation(
		m_windowBounds.Width / 2.0f - m_textMetrics.widthIncludingTrailingWhitespace / 2.0f + m_textPosition.X,
		m_windowBounds.Height / 2.0f - m_textMetrics.height / 2.0f + m_textPosition.Y
		);
	*/

	// Note that the m_orientationTransform2D matrix is post-multiplied here
	// in order to correctly orient the text to match the display orientation.
	// This post-multiplication step is required for any draw calls that are
	// made to the swap chain's target bitmap. For draw calls to other targets,
	// this transform should not be applied.
	/*
	m_d2dContext->SetTransform(translation * m_orientationTransform2D);

	m_d2dContext->DrawTextLayout(
		Point2F(0.0f, 0.0f),
		m_textLayout.Get(),
		m_blackBrush.Get(),
		D2D1_DRAW_TEXT_OPTIONS_NO_SNAP
		);
	*/

	// Ignore D2DERR_RECREATE_TARGET. This error indicates that the device
	// is lost. It will be handled during the next call to Present.
	/*
	HRESULT hr = m_d2dContext->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		DX::ThrowIfFailed(hr);
	}
	*/

	// Below is from Puck old code

	// Retrieve the size of the render target.
	D2D1_SIZE_F renderTargetSize = m_d2dContext->GetSize();

	if (m_game == nullptr)
	{
		// Initialize Game
		m_game = std::unique_ptr<Game>(new Game(b2Vec2(renderTargetSize.width, renderTargetSize.height), m_d2dContext.Get(), m_dwriteFactory.Get()));
	}

	m_d2dContext->BeginDraw();
	m_d2dContext->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	m_d2dContext->SetTransform(D2D1::Matrix3x2F::Identity());

	m_game->Draw();

#ifdef DEBUG
	RenderFPS(renderTargetSize);
#endif

	ThrowIfFailed(
		m_d2dContext->EndDraw()
	);

	//Present();
}

void SimpleTextRenderer::SaveInternalState(IPropertySet^ state)
{
}

void SimpleTextRenderer::LoadInternalState(IPropertySet^ state)
{
}

void SimpleTextRenderer::OnMouseDown(Windows::Foundation::Point p,  bool left, bool right)
{
	m_game->OnMouseDown(b2Vec2(p.X, p.Y), left, right);
}

void SimpleTextRenderer::OnMouseMove(Windows::Foundation::Point p)
{
	m_game->OnMouseMove(b2Vec2(p.X, p.Y));
}

#ifdef DEBUG
void D2DRenderer::RenderFPS(D2D1_SIZE_F)
{
	SYSTEMTIME time;
	GetSystemTime(&time);
	if (time.wSecond != m_lastFrameSecond)
	{
		m_lastFrameSecond = time.wSecond;
		m_fps = m_frameCounter;
		m_frameCounter = 0;

		char buf[20];
		memset(buf, 0, 20);
		sprintf_s(buf, 20, "FPS: %d", m_fps);
		mbstowcs_s(&m_fpsTextLength, m_fpsText, strlen(buf)+1, buf, _TRUNCATE);
	}
	else
	{
		m_frameCounter++;
	}

	ThrowIfFailed(
		m_d2dContext->CreateSolidColorBrush(D2D1::ColorF(1.0, 1.0, 1.0, 0.5), &m_whiteBrush)
		);

	D2D1_RECT_F layoutRect;
	layoutRect.top = 150;
	layoutRect.left = 60;
	layoutRect.bottom = 400;
	layoutRect.right = 400;

	m_d2dContext->DrawText(
		m_fpsText,
		m_fpsTextLength,
		m_textFormat,
		&layoutRect,
		m_whiteBrush);
}
#endif
