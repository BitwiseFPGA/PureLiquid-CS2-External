#pragma once
#include <Math/QAngle.h>
#include <LiquidHookEx/Include.h>
#include <GlobalData/Include.h>

namespace CS2 {
	namespace client {
		class CGameSceneNodeExtended;
	}
}
namespace CS2 {
	class ComputeRandomSeed {
	private:


		inline static LiquidHookEx::Detour m_Hook = LiquidHookEx::Detour("ComputeRandomSeedHook");
		static uint32_t __fastcall hkComputeRandomSeed(CS2::client::CGameSceneNodeExtended* pawn, QAngle_t* angViewAngles, int nPlayerTickCount);
		static void hkComputeRandomSeedEnd();
	public:
		struct ComputeRandomSeedHookData : public LiquidHookEx::Detour::BaseHookData {
			uint32_t m_nRandomSeed;
		};

		static uint32_t GetRandomSeed();
		static bool Hook();
		static bool Unhook();
	};
}