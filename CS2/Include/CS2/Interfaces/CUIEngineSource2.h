#pragma once
#include <Windows.h>
namespace CS2 {
	class CUIPanel;

	class CUIEngineSource2 {
	private:
		using RunScriptFn = __int64(__fastcall*)(CUIEngineSource2*, void* panel, const char* szScript, const char* szScriptName, __int64 a5);

		struct RunScriptContext {
			RunScriptFn pFn;
			void* panel;
			CUIEngineSource2* pUiEngineSource2;
			char *szScript;
		};
		
		static DWORD WINAPI RunScriptThread(LPVOID lpParam);
		static DWORD WINAPI RunScriptThreadEnd();

		inline static RunScriptContext* pRunScriptCtx = nullptr;
		inline static void* pRunScriptShellCodeRemote = nullptr;
		inline static RunScriptFn pRunScriptFn = nullptr;

	public:
		void RunScript(const char* szScript, CUIPanel* panel = nullptr);
	};
}