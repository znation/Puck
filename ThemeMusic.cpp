#include "ThemeMusic.h"
#include "DirectXSample.h"

// Disable warning 4400 (not sure why it's happening)
#pragma warning ( disable : 4400 )

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
	STDMETHODIMP EventNotify(DWORD meEvent, DWORD_PTR param1, DWORD)
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
	m_bstrURL = NULL;

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
		m_spAttributes->SetUnknown(MF_MEDIA_ENGINE_CALLBACK, (IUnknown*) m_spNotify));

	DX::ThrowIfFailed(m_spFactory->CreateInstance(
		MF_MEDIA_ENGINE_WAITFORSTABLE_STATE | MF_MEDIA_ENGINE_AUDIOONLY,
		m_spAttributes,
		&m_spEngine
		));

	DX::ThrowIfFailed(m_spEngine->QueryInterface(__uuidof(IMFMediaEngine), (void**) &m_spEngineEx));

	DX::ThrowIfFailed(m_spEngine->SetLoop(true));
}

ThemeMusic::~ThemeMusic()
{
	delete m_spNotify;
}

void ThemeMusic::Play()
{
	SetFile();
}

void ThemeMusic::SetFile()
{
	auto themeMusic = this;

	StorageFolder^ folder = Windows::ApplicationModel::Package::Current->InstalledLocation;
	StorageFileRetrievalOperation^ op = folder->GetFileAsync("Sound\\PuckTheme.mp3");
	op->Completed = ref new AsyncOperationCompletedHandler<StorageFile^>([themeMusic](IAsyncOperation<StorageFile^>^ fileOp)
	{
		StorageFile^ fileHandle = fileOp->GetResults();
		if (!fileHandle)
		{
			DX::ThrowIfFailed(E_OUTOFMEMORY);
		}

		auto fOpenStream = fileHandle->OpenAsync(Windows::Storage::FileAccessMode::Read);
		if(!fOpenStream)
		{
			DX::ThrowIfFailed(E_OUTOFMEMORY);
		}
		
		themeMusic->SetURL(fileHandle->Path);

		fOpenStream->Completed = ref new AsyncOperationCompletedHandler<IRandomAccessStream^>(
			[themeMusic](IAsyncOperation<IRandomAccessStream^>^ asyncRead)
		{
			auto streamHandle = asyncRead->GetResults();
			themeMusic->SetBytestream(streamHandle);			
		});
		
		if(!fOpenStream->Completed)
		{
			DX::ThrowIfFailed(E_UNEXPECTED);
		}

		fOpenStream->Start();
	});

	op->Start();
}

void ThemeMusic::OnMediaEngineEvent(DWORD meEvent)
{
	switch (meEvent)
	{
	case MF_MEDIA_ENGINE_EVENT_LOADSTART:
		break;
	case MF_MEDIA_ENGINE_EVENT_LOADEDDATA:
		break;
	case MF_MEDIA_ENGINE_EVENT_LOADEDMETADATA:
		break;
	case MF_MEDIA_ENGINE_EVENT_CANPLAY:
		DX::ThrowIfFailed(m_spEngine->Play());
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

void ThemeMusic::SetBytestream(IRandomAccessStream^ streamHandle)
{
	IMFByteStream *spMFByteStream = NULL;
	
	DX::ThrowIfFailed(MFCreateMFByteStreamOnStreamEx((IUnknown*)streamHandle, &spMFByteStream));

	DX::ThrowIfFailed(m_spEngineEx->SetSourceFromByteStream(spMFByteStream, m_bstrURL));	
}

void ThemeMusic::SetURL(Platform::String^ szURL)
{
	if(NULL != m_bstrURL)
	{
		SysFreeString(m_bstrURL);
		m_bstrURL = NULL;
	}

	m_bstrURL = SysAllocString(szURL->Data());
    if (m_bstrURL == 0)
    {
        DX::ThrowIfFailed(E_OUTOFMEMORY);
    }

	return;
}
