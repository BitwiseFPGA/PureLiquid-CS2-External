#pragma once
#include <GlobalData/Include.h>
#include <Math/Vector.h>

using namespace Globals;

namespace CS2 {
	namespace client {
		class C_CSWeaponBaseGun;
	}

	using GetSpreadFn = __int64(__thiscall*)(client::C_CSWeaponBaseGun* pWeapon, void* unknown, void* unknown2);


	struct GetSpreadFnHookData {
		client::C_CSWeaponBaseGun* weapon;
		double flSpread;
	};

	class GetSpread {
	private:



		inline static bool m_bIsHooked = false;
		inline static void* m_pDataRemote = nullptr;
		inline static void* m_pShellcodeRemote = nullptr;
		inline static uintptr_t m_pTargetFunction = 0;

		// Patch Most Common Call
		inline static uint8_t* m_pPatchedMostCommonCallAddr;


		// SetAng Shellcode
		static double __fastcall GetSpread_Hook_Shellcode(
			client::C_CSWeaponBaseGun* pWeapon, void* unknown, void* unknown2);
		static void GetSpread_Hook_Shellcode_End();

	public:
		static bool Hook();
		static bool Unhook();
	};
};