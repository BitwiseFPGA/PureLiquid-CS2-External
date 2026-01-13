#include <RenderObjects/ViewMatrixResolver.h>
#include <GlobalData/Include.h>
#include <Math/Matrix.h>
namespace CS2 {

	uintptr_t ViewMatrixResolver::ResolveViewMatrixAddr() {
		if (!pViewMatrixAddr) {
			auto hClient = ::Globals::proc.GetRemoteModule("client.dll");
			if (!hClient || hClient && !hClient->GetAddr()) {
				return NULL;
			}
			auto getViewMatrixFn = hClient->ScanMemory("48 63 C2 48 8D 0D ?? ?? ?? ?? 48 C1 E0");
			pViewMatrixAddr = hClient->ResolveRIP(getViewMatrixFn + 0x3, 0x3, 0x7);
		}
		return pViewMatrixAddr;
	}
	ViewMatrix_t ViewMatrixResolver::GetViewMatrix()
	{
		ResolveViewMatrixAddr();
		if (!pViewMatrixAddr)
			return ViewMatrix_t();
		return ::Globals::proc.ReadDirect<ViewMatrix_t>(pViewMatrixAddr);
	}
}