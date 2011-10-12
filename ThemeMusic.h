#include <wrl.h>
#include <Mfmediaengine.h>
#include <Mfapi.h>

#pragma once

using namespace Microsoft::WRL;

class MediaEngineNotify;

class ThemeMusic
{
public:
	ThemeMusic(Windows::UI::Core::CoreWindow^ window);
	void OnMediaEngineEvent(DWORD meEvent);

private:
	ComPtr<IMFMediaEngineClassFactory> m_spFactory;
	ComPtr<IMFAttributes> m_spAttributes;
	ComPtr<IMFMediaEngine> m_spEngine;
	ComPtr<MediaEngineNotify> m_spNotify;
};
