#pragma once
#pragma once 
#include <Windows.h>
#include <Memory/Process.h>
#include <GlobalData/xorstr.h>
#include <optional>
#include <vector>
#include <mutex>

namespace Globals {
    extern Process proc;
}

#define thisptr (reinterpret_cast<uintptr_t>(this))

// Original macros (unchanged)
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

#define RUNTIME_OFFSET_PROPERTY(NAME, TYPE, OFFSET_FUNC) \
public: \
    TYPE _internal_Get##NAME() const { \
        uintptr_t offset = OFFSET_FUNC(); \
        return Globals::proc.ReadDirect<TYPE>( \
            reinterpret_cast<uintptr_t>(this) + offset); \
    } \
    void _internal_Set##NAME(TYPE val) { \
        uintptr_t offset = OFFSET_FUNC(); \
        Globals::proc.Write<TYPE>( \
            reinterpret_cast<uintptr_t>(this) + offset, val); \
    } \
    __declspec(property(get = _internal_Get##NAME, put = _internal_Set##NAME)) TYPE NAME;

#define NESTED_PROPERTY(NAME, TYPE, OFFSET) \
public: \
    __forceinline TYPE* _internal_Get##NAME() { \
        return reinterpret_cast<TYPE*>(thisptr + OFFSET); \
    } \
    __declspec(property(get = _internal_Get##NAME)) TYPE* NAME;
#define CACHED_PROPERTY(NAME, TYPE, OFFSET) \
private: \
    static inline std::unordered_map<uintptr_t, TYPE> _cache_##NAME; \
public: \
    __forceinline TYPE _internal_Get##NAME() { \
        auto it = _cache_##NAME.find(thisptr); \
        if (it == _cache_##NAME.end()) { \
            TYPE value = ::Globals::proc.ReadDirect<TYPE>(thisptr + OFFSET); \
            _cache_##NAME[thisptr] = value; \
            return value; \
        } \
        return it->second; \
    } \
    __forceinline void _internal_Set##NAME(TYPE val) { \
        ::Globals::proc.Write<TYPE>(thisptr + OFFSET, val); \
        _cache_##NAME[thisptr] = val; \
    } \
    __forceinline void _internal_Invalidate##NAME() { \
        _cache_##NAME.erase(thisptr); \
    } \
    __declspec(property(get = _internal_Get##NAME, put = _internal_Set##NAME)) TYPE NAME;

#define CACHED_PROPERTY_STRING(NAME, OFFSET, MAX_LENGTH) \
private: \
    static inline std::unordered_map<uintptr_t, std::string> _cache_##NAME; \
public: \
    __forceinline std::string _internal_Get##NAME() { \
        auto it = _cache_##NAME.find(thisptr); \
        if (it == _cache_##NAME.end()) { \
            std::string value = ::Globals::proc.ReadString(thisptr + OFFSET, MAX_LENGTH); \
            _cache_##NAME[thisptr] = value; \
            return value; \
        } \
        return it->second; \
    } \
    __forceinline void _internal_Set##NAME(const std::string& val) { \
        ::Globals::proc.WriteString(thisptr + OFFSET, val, MAX_LENGTH); \
        _cache_##NAME[thisptr] = val; \
    } \
    __forceinline void _internal_Invalidate##NAME() { \
        _cache_##NAME.erase(thisptr); \
    } \
    __declspec(property(get = _internal_Get##NAME, put = _internal_Set##NAME)) std::string NAME;


#define CACHED_PROPERTY_STRING_PTR(NAME, OFFSET, MAX_LENGTH) \
private: \
    static inline std::unordered_map<uintptr_t, std::string> _cache_##NAME; \
public: \
    __forceinline std::string _internal_Get##NAME() { \
        auto it = _cache_##NAME.find(thisptr); \
        if (it == _cache_##NAME.end()) { \
            std::string value = ::Globals::proc.ReadString(proc.ReadDirect<uintptr_t>(thisptr + OFFSET), MAX_LENGTH); \
            _cache_##NAME[thisptr] = value; \
            return value; \
        } \
        return it->second; \
    } \
    __forceinline void _internal_Set##NAME(const std::string& val) { \
        ::Globals::proc.WriteString(proc.ReadDirect<uintptr_t>(thisptr + OFFSET), val, MAX_LENGTH); \
        _cache_##NAME[thisptr] = val; \
    } \
    __forceinline void _internal_Invalidate##NAME() { \
        _cache_##NAME.erase(thisptr); \
    } \
    __declspec(property(get = _internal_Get##NAME, put = _internal_Set##NAME)) std::string NAME;

#define CACHED_PROPERTY_ARRAY(NAME, TYPE, COUNT, OFFSET) \
private: \
    static inline std::unordered_map<uintptr_t, std::vector<TYPE>> _cache_##NAME; \
public: \
    __forceinline std::vector<TYPE> _internal_Get##NAME() { \
        auto it = _cache_##NAME.find(thisptr); \
        if (it == _cache_##NAME.end()) { \
            std::vector<TYPE> value = ::Globals::proc.ReadArray<TYPE>(thisptr + OFFSET, COUNT); \
            _cache_##NAME[thisptr] = value; \
            return value; \
        } \
        return it->second; \
    } \
    __forceinline void _internal_Set##NAME(const std::vector<TYPE>& val) { \
        if (val.size() >= COUNT) { \
            ::Globals::proc.WriteArray<TYPE>(thisptr + OFFSET, val); \
            _cache_##NAME[thisptr] = val; \
        } \
    } \
    __forceinline void _internal_Invalidate##NAME() { \
        _cache_##NAME.erase(thisptr); \
    } \
    __declspec(property(get = _internal_Get##NAME, put = _internal_Set##NAME)) std::vector<TYPE> NAME;

#define CACHED_DYNAMIC_PROPERTY_ARRAY_POINTERS(NAME, TYPE, SIZE_PROPERTY, OFFSET, ...) \
private: \
    static inline std::unordered_map<uintptr_t, std::vector<TYPE>> _cache_##NAME; \
public: \
    __forceinline std::vector<TYPE> _internal_Get##NAME() { \
        auto it = _cache_##NAME.find(thisptr); \
        if (it == _cache_##NAME.end()) { \
            constexpr bool bReadPointerFirst = (sizeof(#__VA_ARGS__) > 1) ? __VA_ARGS__ : true; \
            uint64_t count = this->SIZE_PROPERTY; \
            uintptr_t arrayAddress = thisptr + OFFSET; \
            if constexpr (bReadPointerFirst) { \
                arrayAddress = ::Globals::proc.ReadDirect<uintptr_t>(arrayAddress); \
                if (arrayAddress == 0) return {}; \
            } \
            auto rawPointers = ::Globals::proc.ReadArray<uintptr_t>(arrayAddress, count); \
            std::vector<TYPE> result; \
            result.reserve(rawPointers.size()); \
            for (auto ptr : rawPointers) { \
                result.push_back(reinterpret_cast<TYPE>(ptr)); \
            } \
            _cache_##NAME[thisptr] = result; \
            return result; \
        } \
        return it->second; \
    } \
    __forceinline void _internal_Invalidate##NAME() { \
        _cache_##NAME.erase(thisptr); \
    } \
    __declspec(property(get = _internal_Get##NAME)) std::vector<TYPE> NAME;

#define CACHED_RUNTIME_OFFSET_PROPERTY(NAME, TYPE, OFFSET_FUNC) \
private: \
    static inline std::unordered_map<uintptr_t, TYPE> _cache_##NAME; \
public: \
    TYPE _internal_Get##NAME() const { \
        auto it = _cache_##NAME.find(reinterpret_cast<uintptr_t>(this)); \
        if (it == _cache_##NAME.end()) { \
            uintptr_t offset = OFFSET_FUNC(); \
            TYPE value = Globals::proc.ReadDirect<TYPE>( \
                reinterpret_cast<uintptr_t>(this) + offset); \
            _cache_##NAME[reinterpret_cast<uintptr_t>(this)] = value; \
            return value; \
        } \
        return it->second; \
    } \
    void _internal_Set##NAME(TYPE val) { \
        uintptr_t offset = OFFSET_FUNC(); \
        Globals::proc.Write<TYPE>( \
            reinterpret_cast<uintptr_t>(this) + offset, val); \
        _cache_##NAME[reinterpret_cast<uintptr_t>(this)] = val; \
    } \
    void _internal_Invalidate##NAME() { \
        _cache_##NAME.erase(reinterpret_cast<uintptr_t>(this)); \
    } \
    __declspec(property(get = _internal_Get##NAME, put = _internal_Set##NAME)) TYPE NAME;

#define CACHED_NESTED_PROPERTY(NAME, TYPE, OFFSET) \
private: \
    static inline std::unordered_map<uintptr_t, TYPE*> _cache_##NAME; \
public: \
    __forceinline TYPE* _internal_Get##NAME() { \
        auto it = _cache_##NAME.find(thisptr); \
        if (it == _cache_##NAME.end()) { \
            TYPE* value = reinterpret_cast<TYPE*>(thisptr + OFFSET); \
            _cache_##NAME[thisptr] = value; \
            return value; \
        } \
        return it->second; \
    } \
    __forceinline void _internal_Invalidate##NAME() { \
        _cache_##NAME.erase(thisptr); \
    } \
    __declspec(property(get = _internal_Get##NAME)) TYPE* NAME;