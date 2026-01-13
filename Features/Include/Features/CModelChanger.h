#pragma once
#include <Windows.h>
namespace CS2 {
	namespace client {
		class C_CSPlayerPawn;
	}

	
	class CModelChanger {
		using ChangeModelFn = __int64(__fastcall*)(uintptr_t, const char* szModelName);

		struct ChangeModelContext {
			ChangeModelFn pFn;
			uintptr_t pEntity;
			char szModelName[255];
		};

		static DWORD WINAPI ChangeModelThread(LPVOID lpParam);
		static DWORD WINAPI ChangeModelThreadEnd();

		inline static ChangeModelContext* changeModelRemoteCtx = nullptr;
		inline static void* pChangeModelRemoteShellcode = nullptr;
		inline static ChangeModelFn pChangeModelFn = nullptr;
		static void ChangeModelInternal(uintptr_t pEnt, const char* szModelName);

		
	public:
		template <typename T = client::C_CSPlayerPawn>
		inline static void ChangeModel(T* pEnt, const char* szModelName) {
			ChangeModelInternal(reinterpret_cast<uintptr_t>(pEnt), szModelName);
		}
	};
}