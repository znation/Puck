#include "DirectXViewProvider.h"
#include <windows.h>
#include "D2DRenderer.h"

#ifdef WINRT

[Platform::MTAThread]
int main(array<Platform::String^>^)
{
    auto directXViewProviderFactory = ref new DirectXViewProviderFactory();
    Windows::ApplicationModel::Core::CoreApplication::Run(directXViewProviderFactory);
    return 0;
}

#else

int WINAPI WinMain(
    HINSTANCE /*hInstance*/,
    HINSTANCE /*hPrevInstance*/,
    LPSTR /*lpCmdLine*/,
    int /*nCmdShow*/
    )
{
    // Ignore the return value because we want to run the program even in the
    // unlikely event that HeapSetInformation fails.
    HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
    if (SUCCEEDED(CoInitialize(NULL)))
    {
        {
            D2DRenderer *renderer = new D2DRenderer();
            ThrowIfFailed(renderer->Initialize());
            renderer->RunMessageLoop();
        }
        CoUninitialize();
    }

    return 0;
}

#endif
