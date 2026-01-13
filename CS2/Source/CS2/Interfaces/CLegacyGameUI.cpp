#include <CS2/Interfaces/CLegacyGameUI.h>
#include <GlobalData/Include.h>
using namespace Globals;

namespace CS2 {

#pragma code_seg(".createPopupSeg")
#pragma optimize("", off)
#pragma runtime_checks("", off)
#pragma check_stack(off)  
	DWORD WINAPI CLegacyGameUI::CreatePopupThread(LPVOID lpParam) {
		auto ctx = reinterpret_cast<CreatePopupCtx*>(lpParam);
		// szTitle, szMsg, showOk, showCancel, okCmd, cancelCmd,closeCmd, legend(?), szUnknown
		ctx->pFunc(ctx->pLegacyGameUI,ctx->szTitle,ctx->szMsg,true,false,nullptr,nullptr,nullptr,nullptr,nullptr);
		return 1;
	};

	DWORD WINAPI CLegacyGameUI::CreatePopupThreadEnd() { return 0; }
#pragma check_stack()  
#pragma runtime_checks("", restore) 
#pragma optimize("", on)
#pragma code_seg()

	void CLegacyGameUI::ShowPopup(const char* szTitle, const char* szMessage) {

		if (!pCreatePopupFn) {
			pCreatePopupFn = reinterpret_cast<CreatePopUpFn>(proc.GetVTableFunction<28>(reinterpret_cast<uintptr_t>(this)));
			if (!pCreatePopupFn) {
				printf("Error Finding CreatePopup Function!!\n");
				return;
			}
		}

		if (!pCreatePopupRemoteCtx) {
			pCreatePopupRemoteCtx = reinterpret_cast<CreatePopupCtx*>(proc.Alloc(sizeof(CreatePopupCtx)));
			if (!pCreatePopupRemoteCtx) {
				printf("Error Allocating CreatePopupCtx!!\n");
				return;
			}
			printf("pCreatePopupRemoteCtx: 0x%p\n", reinterpret_cast<uintptr_t>(pCreatePopupRemoteCtx));

		}

		CreatePopupCtx ctx{};
		
		ctx.pFunc = pCreatePopupFn;
		ctx.pLegacyGameUI = this;

		strcpy_s(ctx.szTitle, sizeof(ctx.szTitle), szTitle);
		strcpy_s(ctx.szMsg, sizeof(ctx.szMsg), szMessage);

		if (!proc.Write<CreatePopupCtx>(reinterpret_cast<uintptr_t>(pCreatePopupRemoteCtx), ctx)) {
			printf("Failed to write Run Script context!\n");
			return;
		}
		if (!pCreatePopupRemoteShellcode) {
			pCreatePopupRemoteShellcode = proc.AllocAndWriteShellcode(CreatePopupThread, CreatePopupThreadEnd);
			if (!pCreatePopupRemoteShellcode) {
				printf("Failed to allocate CreatePopup shellcode!\n");
				proc.FreeRemote(pCreatePopupRemoteCtx);
				pCreatePopupRemoteCtx = nullptr;
				return;
			}

			printf("[+] CreatePopup shellcode: 0x%p\n", pCreatePopupRemoteShellcode);
		}
		HANDLE hThread = proc.CreateRemoteThreadEx(
			reinterpret_cast<LPTHREAD_START_ROUTINE>(pCreatePopupRemoteShellcode),
			pCreatePopupRemoteCtx
		);

		if (!hThread) {
			printf("Failed to create CreatePopup thread!\n");
			proc.FreeRemote(pCreatePopupRemoteShellcode);
			proc.FreeRemote(pCreatePopupRemoteCtx);
			pCreatePopupRemoteCtx = nullptr;
			pCreatePopupRemoteShellcode = nullptr;
			return;
		}

		DWORD waitResult = WaitForSingleObject(hThread, 10000);

		if (waitResult != WAIT_OBJECT_0) {
			printf("CreatePopup Thread wait failed or timed out! Result: %d\n", waitResult);
			CloseHandle(hThread);
			proc.FreeRemote(pCreatePopupRemoteShellcode);
			proc.FreeRemote(pCreatePopupRemoteCtx);
			pCreatePopupRemoteCtx = nullptr;
			pCreatePopupRemoteShellcode = nullptr;
			return;
		}

		DWORD exitCode = 0;
		GetExitCodeThread(hThread, &exitCode);
		CloseHandle(hThread);
	}
}