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
    auto directXViewProviderFactory = ref new DirectXViewProviderFactory();
#ifdef WINRT
    Windows::ApplicationModel::Core::CoreApplication::Run(directXViewProviderFactory);
#endif
    return 0;
}
