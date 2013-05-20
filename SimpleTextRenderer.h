#pragma once

#include "DirectXBase.h"
#include "Objects/Game.h"

// This class renders simple text with a colored background.
ref class SimpleTextRenderer sealed : public DirectXBase
{
public:
	SimpleTextRenderer();

	// DirectXBase methods.
	virtual void CreateDeviceIndependentResources() override;
	virtual void CreateDeviceResources() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void Render() override;

	// Method for updating time-dependent objects.
	void Update(float timeTotal, float timeDelta);

	// Methods to save and load state in response to suspend.
	void SaveInternalState(Windows::Foundation::Collections::IPropertySet^ state);
	void LoadInternalState(Windows::Foundation::Collections::IPropertySet^ state);

	void OnMouseMove(Windows::Foundation::Point p);
	void OnMouseDown(Windows::Foundation::Point p, bool isLeftPressed, bool isRightPressed);

private:
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_blackBrush;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_textFormat;
	Microsoft::WRL::ComPtr<IDWriteTextLayout> m_textLayout;
	DWRITE_TEXT_METRICS m_textMetrics;
	Windows::Foundation::Point m_textPosition;
	bool m_renderNeeded;
	int m_backgroundColorIndex;

	// for game
	std::unique_ptr<Game> m_game;
	void RenderFPS(D2D1_SIZE_F);
};
