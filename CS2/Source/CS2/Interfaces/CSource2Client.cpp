#include <CS2/Interfaces/CSource2Client.h>
#include <GlobalData/Include.h>
using namespace Globals;
namespace CS2 {
	CSceneViewDebugOverlays* CSource2Client::GetSceneDebugOverlay()
	{
		// info: makes use of DLL Cloning. no need to CreateRemoteThread here, as it is a simple
		// function and DLL is cloned in this process. We can execute remote functions here,
		// and get the same result as in the remote process!
		using GetCSceneViewDebugOverlaysFn = CSceneViewDebugOverlays*(__fastcall*)();
		return reinterpret_cast<GetCSceneViewDebugOverlaysFn>(proc.GetVTableFunction<160>(reinterpret_cast<uintptr_t>(this)))();
	}
}