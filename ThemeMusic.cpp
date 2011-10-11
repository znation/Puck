#include "ThemeMusic.h"
#include <Mfmediaengine.h>
#include <Mfapi.h>
#include "DirectXSample.h"

// MediaEngineNotify: Implements the callback for Media Engine event notification.
class MediaEngineNotify : public IMFMediaEngineNotify
{
    long m_cRef;
    Windows::UI::Core::CoreWindow^ m_cWindow;

public:
    MediaEngineNotify(Windows::UI::Core::CoreWindow^ cWindow) : m_cWindow(cWindow), m_cRef(1)
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
			// TODO event here
			//m_pCB->OnMediaEngineEvent(meEvent);			
		}

		return S_OK;
    }
};

void ThemeMusic::Play(Windows::UI::Core::CoreWindow^ window)
{
	// TODO throw if these fail

	ComPtr<IMFMediaEngineClassFactory> spFactory;
	ComPtr<IMFAttributes> spAttributes;
	ComPtr<IMFMediaEngine> ppPlayer;
	ComPtr<MediaEngineNotify> spNotify;

	DX::ThrowIfFailed(
		MFStartup(
		MF_VERSION
		));

	// Create our event callback object.
    spNotify = new MediaEngineNotify(window);
    if (spNotify == NULL)
    {
        DX::ThrowIfFailed(E_OUTOFMEMORY);    
    }

	DX::ThrowIfFailed(
		CoCreateInstance(
		CLSID_MFMediaEngineClassFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&spFactory)));

	DX::ThrowIfFailed(
		MFCreateAttributes(&spAttributes, 1));

	DX::ThrowIfFailed(
		spAttributes->SetUnknown(MF_MEDIA_ENGINE_CALLBACK, (IUnknown*) spNotify.Get()));

	DX::ThrowIfFailed(spFactory->CreateInstance(
		MF_MEDIA_ENGINE_WAITFORSTABLE_STATE | MF_MEDIA_ENGINE_AUDIOONLY,
		spAttributes.Get(),
		&ppPlayer
		));
}

