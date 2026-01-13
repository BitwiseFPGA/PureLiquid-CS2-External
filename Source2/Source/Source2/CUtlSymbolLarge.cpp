#include <GlobalData/Include.h>
#include <Source2/CUtlSymbolLarge.h>
#include <vector>

std::string Source2::CUtlSymbolLarge::Get() const {
	return ::Globals::proc.ReadString(reinterpret_cast<uintptr_t>(this) + offsetof(CUtlSymbolLarge, _str));
}