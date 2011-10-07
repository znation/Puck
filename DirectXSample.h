//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

namespace DX
{
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            __cli_WinRTraiseException(hr);
        }
    }

    inline IUnknown* GetIUnknown(Windows::UI::Core::CoreWindow^ window)
    {
        return (IUnknown*) reinterpret_cast<__cli_IUnknown*>(window);
    }
}
