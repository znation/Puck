#ifdef WINRT
#include <wrl.h>
#endif
#include "Box2D\Box2D.h"
#include <d2d1_1helper.h>
#include <d2d1_1.h>

#pragma once

#ifdef WINRT
using namespace Microsoft::WRL;
using namespace Windows::Storage;
using namespace Windows::Foundation;
using namespace Windows::Storage::Streams;
#else
#define ref 
#endif

#ifdef DEBUG
#define MAX_SCORE 2
#else
#define MAX_SCORE 11
#endif

#define MENU_BUTTON_COUNT 2
#define FRAMERATE 60.0f

float32 pixelToBox(float32 coord);
float32 boxToPixel(float32 coord);
bool rectContainsPoint(D2D1_RECT_F rect, b2Vec2 point);
void ThrowIfFailed(HRESULT hr);

#ifdef WINRT
IUnknown* GetIUnknown(Windows::UI::Core::CoreWindow^ window);
#else
// 
// This structs acts as a smart pointer for IUnknown pointers 
// making sure to call AddRef() and Release() as needed. 
// 
template<typename T> 
struct ComPtr 
{ 
public: 
 
    ComPtr(T* lComPtr = nullptr) : m_ComPtr(lComPtr) 
    { 
        static_assert(std::tr1::is_base_of<IUnknown, T>::value, "T needs to be IUnknown based"); 
        if (m_ComPtr) 
        { 
            m_ComPtr->AddRef(); 
        } 
    } 
 
    ComPtr(const ComPtr<T>& lComPtrObj) 
    { 
        static_assert(std::tr1::is_base_of<IUnknown, T>::value, "T needs to be IUnknown based"); 
        m_ComPtr = lComPtrObj.m_ComPtr; 
 
        if (m_ComPtr) 
        { 
            m_ComPtr->AddRef(); 
        } 
    } 
 
    ComPtr(ComPtr<T>&& lComPtrObj) 
    { 
        m_ComPtr = lComPtrObj.m_ComPtr; 
        lComPtrObj.m_ComPtr = nullptr; 
    } 
 
    T* operator=(T* lComPtr) 
    { 
        if (m_ComPtr) 
        { 
            m_ComPtr->Release(); 
        } 
 
        m_ComPtr = lComPtr; 
 
        if (m_ComPtr) 
        { 
            m_ComPtr->AddRef(); 
        } 
 
        return m_ComPtr; 
    } 
 
    T* operator=(const ComPtr<T>& lComPtrObj) 
    { 
        if (m_ComPtr) 
        { 
            m_ComPtr->Release(); 
        } 
 
        m_ComPtr = lComPtrObj.m_ComPtr; 
 
        if (m_ComPtr) 
        { 
            m_ComPtr->AddRef(); 
        } 
 
        return m_ComPtr; 
    } 
 
    ~ComPtr() 
    { 
        if (m_ComPtr) 
        { 
            m_ComPtr->Release(); 
            m_ComPtr = nullptr; 
        } 
    } 
 
    operator T*() const 
    { 
        return m_ComPtr; 
    } 
 
    T* GetInterface() const 
    { 
        return m_ComPtr; 
    } 
 
    T& operator*() const 
    { 
        return *m_ComPtr; 
    } 
 
    T** operator&() 
    { 
        //The assert on operator& usually indicates a bug. Could be a potential memory leak. 
        // If this really what is needed, however, use GetInterface() explicitly. 
        assert(nullptr == m_ComPtr); 
        return &m_ComPtr; 
    } 
 
    T* operator->() const 
    { 
        return m_ComPtr; 
    } 
 
    bool operator!() const 
    {     
        return (nullptr == m_ComPtr); 
    } 
 
    bool operator<(T* lComPtr) const 
    { 
        return m_ComPtr < lComPtr; 
    } 
 
    bool operator!=(T* lComPtr) const 
    { 
        return !operator==(lComPtr); 
    } 
 
    bool operator==(T* lComPtr) const 
    { 
        return m_ComPtr == lComPtr; 
    } 
 
    template <typename I> 
    HRESULT QueryInterface(I **interfacePtr) 
    { 
        return m_ComPtr->QueryInterface(IID_PPV_ARGS(interfacePtr)); 
	}

	T * Get()
	{
		return m_ComPtr;
	}

protected:
    // The internal interface pointer 
    T* m_ComPtr; 
};
#endif
