#include <GlobalData/Include.h>
#include <Features/CModelChanger.h>
#include <CS2/Interfaces/Include.h>

using namespace Globals;

namespace CS2 {

#pragma code_seg(".changeModelSeg")
#pragma optimize("", off)
#pragma runtime_checks("", off)
#pragma check_stack(off)  
	DWORD WINAPI CModelChanger::ChangeModelThread(LPVOID lpParam) {
		auto ctx = reinterpret_cast<ChangeModelContext*>(lpParam);
		ctx->pFn(ctx->pEntity,ctx->szModelName);
		return 1;
	};

	DWORD WINAPI CModelChanger::ChangeModelThreadEnd() { return 0; }
#pragma check_stack()  
#pragma runtime_checks("", restore) 
#pragma optimize("", on)
#pragma code_seg()


	void CModelChanger::ChangeModelInternal(uintptr_t pEnt, const char* szModelName) {


		if (!pChangeModelFn) {


			auto pPanorama = proc.GetRemoteModule("client.dll");
			if (!pPanorama) {
				printf("ERROR: Failed to get client.dll\n");
				return;
			}

			pChangeModelFn = reinterpret_cast<ChangeModelFn>(pPanorama->ScanMemory(CHANGE_MODEL_PATTERN));
			if (!pChangeModelFn) {
				printf("Error Finding ChangeModel Function!!\n");
				return;
			}
		}

		if (!changeModelRemoteCtx) {
			changeModelRemoteCtx = reinterpret_cast<ChangeModelContext*>(proc.Alloc(sizeof(ChangeModelContext)));
			if (!changeModelRemoteCtx) {
				printf("Error Allocating ChangeModelCtx!!\n");
				return;
			}
			printf("ChangeModel Ctx: 0x%p\n", reinterpret_cast<uintptr_t>(changeModelRemoteCtx));

		}

		ChangeModelContext ctx{};
		ctx.pFn = pChangeModelFn;
		ctx.pEntity = pEnt;

		strcpy_s(ctx.szModelName, sizeof(ctx.szModelName), szModelName);

		if (!proc.Write<ChangeModelContext>(reinterpret_cast<uintptr_t>(changeModelRemoteCtx), ctx)) {
			printf("Failed to write Run Script context!\n");
			return;
		}
		if (!pChangeModelRemoteShellcode) {
			pChangeModelRemoteShellcode = proc.AllocAndWriteShellcode(ChangeModelThread, ChangeModelThreadEnd);
			if (!pChangeModelRemoteShellcode) {
				printf("Failed to allocate ChangeModel shellcode!\n");
				proc.FreeRemote(changeModelRemoteCtx);
				changeModelRemoteCtx = nullptr;
				return;
			}

			printf("[+] ChangeModel shellcode: 0x%p\n", pChangeModelRemoteShellcode);
		}
		HANDLE hThread = proc.CreateRemoteThreadEx(
			reinterpret_cast<LPTHREAD_START_ROUTINE>(pChangeModelRemoteShellcode),
			changeModelRemoteCtx
		);

		if (!hThread) {
			printf("Failed to create ChangeModel thread!\n");
			proc.FreeRemote(pChangeModelRemoteShellcode);
			proc.FreeRemote(changeModelRemoteCtx);
			changeModelRemoteCtx = nullptr;
			pChangeModelRemoteShellcode = nullptr;
			return;
		}

		DWORD waitResult = WaitForSingleObject(hThread, 10000);

		if (waitResult != WAIT_OBJECT_0) {
			printf("ChangeModel Thread wait failed or timed out! Result: %d\n", waitResult);
			CloseHandle(hThread);
			proc.FreeRemote(pChangeModelRemoteShellcode);
			proc.FreeRemote(changeModelRemoteCtx);
			changeModelRemoteCtx = nullptr;
			pChangeModelRemoteShellcode = nullptr;
			return;
		}

		DWORD exitCode = 0;
		GetExitCodeThread(hThread, &exitCode);
		CloseHandle(hThread);
	}
}