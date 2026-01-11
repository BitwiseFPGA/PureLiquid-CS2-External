#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>
namespace CS2 {
	class CCSGO_Hud {
	public:
		uintptr_t FindHudElement(const std::string& elementName, DWORD sub = 0x20);

		template <typename T>
		T FindHudElement(const std::string& elementName, DWORD sub = 0x20) {
			return reinterpret_cast<T>(FindHudElement(elementName, sub));
		}
	};
}