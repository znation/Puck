#include "DirectXViewProvider.h"

#ifdef WINRT

[Platform::MTAThread]
int main(array<Platform::String^>^)
{
    auto directXViewProviderFactory = ref new DirectXViewProviderFactory();
    Windows::ApplicationModel::Core::CoreApplication::Run(directXViewProviderFactory);
    return 0;
}

#else

int main()
{
	return 0;
}

#endif
