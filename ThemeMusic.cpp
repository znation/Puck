#include "ThemeMusic.h"
#include "DirectXSample.h"

// MediaEngineNotify: Implements the callback for Media Engine event notification.
class MediaEngineNotify : public IMFMediaEngineNotify
{
private:
	long m_cRef;
	Windows::UI::Core::CoreWindow^ m_cWindow;
	ThemeMusic *m_tM;

public:
	MediaEngineNotify(Windows::UI::Core::CoreWindow^ cWindow, ThemeMusic *tM) : m_cWindow(cWindow), m_cRef(1), m_tM(tM)
	{
	}

	STDMETHODIMP QueryInterface(REFIID riid, void** ppv)
	{
		if(__uuidof(IMFMediaEngineNotify) == riid)
		{
			*ppv = static_cast<IMFMediaEngineNotify*>(this);
		}
		else
		{
			*ppv = NULL;
			return E_NOINTERFACE;
		}

		AddRef();

		return S_OK;
	}      

	STDMETHODIMP_(ULONG) AddRef()
	{
		return InterlockedIncrement(&m_cRef);
	}

	STDMETHODIMP_(ULONG) Release()
	{
		LONG cRef = InterlockedDecrement(&m_cRef);
		if (cRef == 0)
		{
			delete this;
		}
		return cRef;
	}

	// EventNotify is called when the Media Engine sends an event.
	STDMETHODIMP EventNotify(DWORD meEvent, DWORD_PTR param1, DWORD param2)
	{
		if (meEvent == MF_MEDIA_ENGINE_EVENT_NOTIFYSTABLESTATE)
		{
			SetEvent(reinterpret_cast<HANDLE>(param1));			
		}
		else
		{
			m_tM->OnMediaEngineEvent(meEvent);
		}

		return S_OK;
	}
};

ThemeMusic::ThemeMusic(Windows::UI::Core::CoreWindow^ window)
{
	DX::ThrowIfFailed(
		MFStartup(
		MF_VERSION
		));

	// Create our event callback object.
	m_spNotify = new MediaEngineNotify(window, this);
	if (m_spNotify == NULL)
	{
		DX::ThrowIfFailed(E_OUTOFMEMORY);    
	}

	DX::ThrowIfFailed(
		CoCreateInstance(
		CLSID_MFMediaEngineClassFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_spFactory)));

	DX::ThrowIfFailed(
		MFCreateAttributes(&m_spAttributes, 1));

	DX::ThrowIfFailed(
		m_spAttributes->SetUnknown(MF_MEDIA_ENGINE_CALLBACK, (IUnknown*) m_spNotify.Get()));

	DX::ThrowIfFailed(m_spFactory->CreateInstance(
		MF_MEDIA_ENGINE_WAITFORSTABLE_STATE | MF_MEDIA_ENGINE_AUDIOONLY,
		m_spAttributes.Get(),
		&m_spEngine
		));

	DX::ThrowIfFailed(m_spEngine->SetSource(L"Sound\\PuckTheme.mp3"));
}

void ThemeMusic::OnMediaEngineEvent(DWORD meEvent)
{
	switch (meEvent)
	{
	case MF_MEDIA_ENGINE_EVENT_LOADEDMETADATA:
		break;
	case MF_MEDIA_ENGINE_EVENT_CANPLAY:
		{
			DX::ThrowIfFailed(m_spEngine->Play());
		}
		break;        
	case MF_MEDIA_ENGINE_EVENT_PLAY:
		break;				
	case MF_MEDIA_ENGINE_EVENT_PAUSE:
		break;
	case MF_MEDIA_ENGINE_EVENT_TIMEUPDATE:        
		break;
	case MF_MEDIA_ENGINE_EVENT_ERROR:
		DX::ThrowIfFailed(E_ABORT);
		break;    
	}
}
