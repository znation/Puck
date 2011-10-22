#include "DirectXViewProvider.h"

#ifdef WINRT
[Platform::MTAThread]
int main(array<Platform::String^>^)
#else
int main()
#endif
{
    auto directXViewProviderFactory = ref new DirectXViewProviderFactory();
#ifdef WINRT
    Windows::ApplicationModel::Core::CoreApplication::Run(directXViewProviderFactory);
#endif
    return 0;
}
