#include <GlobalData/Include.h>
#include <Source2/CUtlString.h>
#include <vector>
#include <string>

std::string Source2::CUtlString::Get() const {
	return ::Globals::proc.ReadString(reinterpret_cast<uintptr_t>(this) + offsetof(CUtlString, _str));
}