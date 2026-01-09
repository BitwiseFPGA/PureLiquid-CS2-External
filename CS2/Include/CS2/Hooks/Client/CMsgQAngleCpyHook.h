#pragma once
#include <Math/QAngle.h>
#include <Math/Vector.h>
namespace CS2 {


	class CMsgQAngleTest
	{
	public:
		class CBasePB* base; //0x00
		char pad_0008[16]; //0x08
		QAngle angValue; //0x18
		char pad_0024[92]; //0x24
	}; //Size: 0x0080

	using OriginalCpyQAngleFnDef = __int64(__thiscall*)(CMsgQAngleTest* to, CMsgQAngleTest* from);
	
	struct CMsgQAngleCpyHookData {
		OriginalCpyQAngleFnDef pOriginal;
		Vector qAngleToSet;
		bool bForceQAngle;
	};

	class CMsgQAngleCpy {
	private :
		inline static bool m_bIsHooked = false;
		inline static void* m_pDataRemote = nullptr;
		inline static void* m_pShellcodeRemote = nullptr;
		inline static uintptr_t m_pTargetFunction = 0;

		// Patch Most Common Call
		inline static uint8_t* m_pPatchedMostCommonCallAddr;


		// SetAng Shellcode
		static void __fastcall SetAng_Hook_Shellcode(
			uintptr_t a1, CMsgQAngleTest* a2);
		static void SetAng_Hook_Shellcode_End();

	public:
		static bool Hook();
		static bool Unhook();
		static void ToggleAngleChange();
		static void SetAngleChange(bool bEnabled);
		static void SetAngle(Vector vAngle);
	};
}