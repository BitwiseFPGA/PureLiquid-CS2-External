#include <CS2/Interfaces/CPanoramaUIEngine.h>
#include <GlobalData/Include.h>

namespace CS2 {

	CUIEngineSource2* CPanoramaUIEngine::GetCUIEngineSource2() {
		return Globals::proc.ReadDirect<CUIEngineSource2*>(reinterpret_cast<uintptr_t>(this) + 0x28);
	}
}