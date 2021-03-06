#include "DirectXViewProvider.h"
#include <windows.h>
#include "D2DRenderer.h"

[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
    auto directXViewProviderFactory = ref new DirectXViewProviderFactory();
    Windows::ApplicationModel::Core::CoreApplication::Run(directXViewProviderFactory);
    return 0;
}