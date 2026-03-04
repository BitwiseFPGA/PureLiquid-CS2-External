#pragma once
#include <GlobalData/Include.h>
#include <Math/Vector.h>
#include <Memory/HookConfig.h>

using namespace Globals;

namespace CS2 {
	namespace client {
		class C_CSWeaponBaseGun;
	}

	using GetInaccuracyFn = float(__thiscall*)(client::C_CSWeaponBaseGun* pWeapon, void* unknown, void* unknown2);


	struct GetInaccuracyFnHookData {
		client::C_CSWeaponBaseGun* weapon;
		float flInaccuracy;
	};

	class GetInaccuracy {
	private:



		inline static bool m_bIsHooked = false;
		inline static void* m_pDataRemote = nullptr;
		inline static void* m_pShellcodeRemote = nullptr;
		inline static uintptr_t m_pTargetFunction = 0;

		// Patch Most Common Call
		inline static uint8_t* m_pPatchedMostCommonCallAddr;


		// SetAng Shellcode
		static float __fastcall GetInaccuracy_Hook_Shellcode(
			client::C_CSWeaponBaseGun* pWeapon, void* unknown, void* unknown2);
		static void GetInaccuracy_Hook_Shellcode_End();

	public:
		static bool Hook();
		static bool Unhook();
		static bool TryRestore();
		static GetInaccuracyFnHookData GetData();
	};
};