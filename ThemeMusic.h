#include <wrl.h>

#pragma once

using namespace Microsoft::WRL;

class ThemeMusic
{
public:
	static void Play(Windows::UI::Core::CoreWindow^ window);
};
