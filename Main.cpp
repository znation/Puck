#include "DirectXViewProvider.h"

#ifdef WINRT
[Platform::MTAThread]
int main(array<Platform::String^>^)
#else
int __stdcall WinMain(__in  HINSTANCE hInstance,
  __in  HINSTANCE hPrevInstance,
  __in  LPSTR lpCmdLine,
  __in  int nCmdShow)
#endif
{
#ifdef WINRT
    auto directXViewProviderFactory = ref new DirectXViewProviderFactory();
    Windows::ApplicationModel::Core::CoreApplication::Run(directXViewProviderFactory);
#else
	DirectXViewProvider *provider = new DirectXViewProvider();
	provider->Run();
	delete provider;
#endif
    return 0;
}
