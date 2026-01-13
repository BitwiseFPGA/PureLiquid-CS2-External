#include <CS2/Interfaces/CUIEngineSource2.h>
#include <CS2/Patterns.h>
#include <GlobalData/Include.h>



using namespace Globals;

namespace CS2 {



#pragma code_seg(".runScriptSeg")
#pragma optimize("", off)
#pragma runtime_checks("", off)
#pragma check_stack(off)  
	DWORD WINAPI CUIEngineSource2::RunScriptThread(LPVOID lpParam) {
		auto ctx = reinterpret_cast<RunScriptContext*>(lpParam);
		ctx->pFn(ctx->pUiEngineSource2, ctx->panel, ctx->szScript, nullptr, 1);
		return 1;
	};

	DWORD WINAPI CUIEngineSource2::RunScriptThreadEnd() { return 0; }
#pragma check_stack()  
#pragma runtime_checks("", restore) 
#pragma optimize("", on)
#pragma code_seg()


	void CUIEngineSource2::RunScript(const char* szScript, CUIPanel* panel) {


		if (!pRunScriptFn) {


			auto pPanorama = proc.GetRemoteModule("panorama.dll");
			if (!pPanorama) {
				printf("ERROR: Failed to get panorama.dll\n");
				return;
			}

			pRunScriptFn = reinterpret_cast<RunScriptFn>(pPanorama->ScanMemory(RUN_SCRIPT_PATTERN));
			if (!pRunScriptFn) {
				printf("Error Finding RunScript Function!!\n");
				return;
			}
		}

		if (!pRunScriptCtx) {
			pRunScriptCtx = reinterpret_cast<RunScriptContext*>(proc.Alloc(sizeof(RunScriptContext)));
			if (!pRunScriptCtx) {
				printf("Error Allocating RunScriptCtx!!\n");
				return;
			}
			printf("RunScriptCtx: 0x%p\n", reinterpret_cast<uintptr_t>(pRunScriptCtx));

		}

		RunScriptContext ctx{};
		ctx.pFn = pRunScriptFn;
		ctx.panel = panel ? reinterpret_cast<void*>(panel) : 0x0;
		ctx.pUiEngineSource2 = this;
		auto remoteScriptBuffer = proc.AllocateAndWriteString(szScript);
		ctx.szScript = reinterpret_cast<char*>(remoteScriptBuffer);
		// strcpy_s(ctx.szScript, sizeof(ctx.szScript), szScript);
		
		if (!proc.Write<RunScriptContext>(reinterpret_cast<uintptr_t>(pRunScriptCtx), ctx)) {
			printf("Failed to write Run Script context!\n");
			return;
		}
		if (!pRunScriptShellCodeRemote) {
			pRunScriptShellCodeRemote = proc.AllocAndWriteShellcode(RunScriptThread, RunScriptThreadEnd);
			if (!pRunScriptShellCodeRemote) {
				printf("Failed to allocate RunScript shellcode!\n");
				proc.FreeRemote(pRunScriptCtx);
				proc.FreeRemote(remoteScriptBuffer);

				pRunScriptCtx = nullptr;
				return;
			}

			printf("[+] RunScript shellcode: 0x%p\n", pRunScriptShellCodeRemote);
		}
		HANDLE hThread = proc.CreateRemoteThreadEx(
			reinterpret_cast<LPTHREAD_START_ROUTINE>(pRunScriptShellCodeRemote),
			pRunScriptCtx
		);

		if (!hThread) {
			printf("Failed to create RunScript thread!\n");
			proc.FreeRemote(pRunScriptShellCodeRemote);
			proc.FreeRemote(pRunScriptCtx);
			proc.FreeRemote(remoteScriptBuffer);

			pRunScriptCtx = nullptr;
			pRunScriptShellCodeRemote = nullptr;
			return;
		}

		DWORD waitResult = WaitForSingleObject(hThread, 10000);

		if (waitResult != WAIT_OBJECT_0) {
			printf("RunScript Thread wait failed or timed out! Result: %d\n", waitResult);
			CloseHandle(hThread);
			proc.FreeRemote(pRunScriptShellCodeRemote);
			proc.FreeRemote(pRunScriptCtx);
			proc.FreeRemote(remoteScriptBuffer);

			pRunScriptCtx = nullptr;
			pRunScriptShellCodeRemote = nullptr;
			return;
		}

		DWORD exitCode = 0;
		GetExitCodeThread(hThread, &exitCode);
		CloseHandle(hThread);
		proc.FreeRemote(remoteScriptBuffer);
		/* Simple Fire and cleanup execution. - the previous logic is just for re - using the same RemoteCtx& Shellcode!!
		DWORD result = proc.ExecuteRemoteWrapper(
			RunScriptThread,
			RunScriptThreadEnd,
			ctx
		);*/
	}
}