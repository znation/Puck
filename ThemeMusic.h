#ifdef WINRT

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
	~ThemeMusic();
	void OnMediaEngineEvent(DWORD meEvent);
	void Play();

private:
	void SetFile();
	void SetBytestream(IRandomAccessStream^ streamHandle);
	void SetURL(Platform::String^ szURL);
	IMFMediaEngineClassFactory * m_spFactory;
	IMFAttributes * m_spAttributes;
	IMFMediaEngine * m_spEngine;
	IMFMediaEngineEx * m_spEngineEx;
	MediaEngineNotify * m_spNotify;
	BSTR m_bstrURL;
};

#endif
