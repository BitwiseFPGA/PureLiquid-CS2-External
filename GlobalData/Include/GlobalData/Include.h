#pragma once 
#include <Windows.h>
#include <Memory/Process.h>
namespace Globals {
	extern Process proc;
}


#define thisptr (reinterpret_cast<uintptr_t>(this))

#define PROPERTY(NAME, TYPE, OFFSET) \
public: \
    __forceinline TYPE _internal_Get##NAME() { \
        return ::Globals::proc.ReadDirect<TYPE>(thisptr + OFFSET); \
    } \
    __forceinline void _internal_Set##NAME(TYPE val) { \
        ::Globals::proc.Write<TYPE>(thisptr + OFFSET, val); \
    } \
    __declspec(property(get = _internal_Get##NAME, put = _internal_Set##NAME)) TYPE NAME;

#define PROPERTY_ARRAY(NAME, TYPE, COUNT, OFFSET) \
public: \
    __forceinline std::vector<TYPE> _internal_Get##NAME() { \
        return ::Globals::proc.ReadArray<TYPE>(thisptr + OFFSET, COUNT); \
    } \
    __forceinline void _internal_Set##NAME(const std::vector<TYPE>& val) { \
        if (val.size() >= COUNT) \
            ::Globals::proc.WriteArray<TYPE>(thisptr + OFFSET, val); \
    } \
    __declspec(property(get = _internal_Get##NAME, put = _internal_Set##NAME)) std::vector<TYPE> NAME;

#define RUNTIME_OFFSET_PROPERTY(NAME, TYPE, OFFSET_FUNC)           \
public:                                                             \
    TYPE _internal_Get##NAME() const {                              \
        uintptr_t offset = OFFSET_FUNC();                           \
        return Globals::proc.ReadDirect<TYPE>(                      \
            reinterpret_cast<uintptr_t>(this) + offset);           \
    }                                                               \
    void _internal_Set##NAME(TYPE val) {                            \
        uintptr_t offset = OFFSET_FUNC();                           \
        Globals::proc.Write<TYPE>(                                  \
            reinterpret_cast<uintptr_t>(this) + offset, val);       \
    }                                                               \
    __declspec(property(get = _internal_Get##NAME, put = _internal_Set##NAME)) TYPE NAME;