#include <RenderObjects/ViewMatrixResolver.h>
#include <GlobalData/Include.h>
#include <CS2/Patterns.h>
#include <Math/Matrix.h>
namespace CS2 {

	uintptr_t ViewMatrixResolver::ResolveViewMatrixAddr() {
		if (!pViewMatrixAddr) {
			auto hClient = ::Globals::proc.GetRemoteModule("client.dll");
			if (!hClient || hClient && !hClient->GetAddr()) {
				return NULL;
			}
			auto getViewMatrixFn = hClient->ScanMemory(GET_VIEW_MATRIX_FN);
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