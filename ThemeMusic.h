#include <wrl.h>
#include <Mfmediaengine.h>
#include <Mfapi.h>

#pragma once

using namespace Microsoft::WRL;
using namespace Windows::Storage;
using namespace Windows::Foundation;
using namespace Windows::Storage::Streams;

class MediaEngineNotify;

class ThemeMusic
{
public:
	ThemeMusic(Windows::UI::Core::CoreWindow^ window);
	void OnMediaEngineEvent(DWORD meEvent);
	void Play();

private:
	void SetFile();
	void SetBytestream(IRandomAccessStream^ streamHandle);
	void SetURL(Platform::String^ szURL);
	ComPtr<IMFMediaEngineClassFactory> m_spFactory;
	ComPtr<IMFAttributes> m_spAttributes;
	ComPtr<IMFMediaEngine> m_spEngine;
	ComPtr<IMFMediaEngineEx> m_spEngineEx;
	ComPtr<MediaEngineNotify> m_spNotify;
	BSTR m_bstrURL;
};
