#pragma once
#include <Windows.h>
namespace CS2 {
	class CLegacyGameUI {
	private:
		using CreatePopUpFn = __int64(__fastcall*)(CLegacyGameUI*, const char*, const char*, bool, bool, const char*, const char*, const char*, void*, const char*);

		struct CreatePopupCtx {
			CLegacyGameUI* pLegacyGameUI;
			CreatePopUpFn pFunc;
			char szTitle[255];
			char szMsg[255];
		};

		inline static CreatePopupCtx* pCreatePopupRemoteCtx = nullptr;
		inline static void* pCreatePopupRemoteShellcode = nullptr;
		inline static CreatePopUpFn pCreatePopupFn = nullptr;

		static DWORD WINAPI CreatePopupThread(LPVOID lpParam);
		static DWORD WINAPI CreatePopupThreadEnd();

	public:
		// void ShowPopup(const char* szTitle, const char* szMessage, bool bShowOk = true, bool bShowCancel = false, const char* pOkCmd = nullptr, const char* pCancelCmd = nullptr, const char* pClosedCmd = nullptr, void* pLegend = nullptr, const char* szUnknown = nullptr);
		void ShowPopup(const char* szTitle, const char* szMessage);

	};
}