#include <Cs2/Hooks/Client/ComputeRandomSeed.h>
#include <CS2/ExtendedSDK/client/C_CSPlayerPawn.h>
#include <CS2/Patterns.h>

namespace CS2 {
    static void* g_pOriginalComputeRandomSeed = nullptr;
    static ComputeRandomSeed::ComputeRandomSeedHookData* g_pComputeRandomSeedHookData = nullptr;

    LH_START(".hkComputeRandomSeed")

        uint32_t __fastcall ComputeRandomSeed::hkComputeRandomSeed(
            CS2::client::CGameSceneNodeExtended* pawn, QAngle_t* angViewAngles, int nPlayerTickCount)
    {
        volatile CS2::client::CGameSceneNodeExtended* va1 = pawn;
        volatile QAngle_t* va2 = angViewAngles;
        volatile int va3 = nPlayerTickCount;

        volatile ComputeRandomSeed::ComputeRandomSeedHookData* data = g_pComputeRandomSeedHookData;

        typedef uint32_t(__fastcall* ComputeRandomSeedFn)(CS2::client::CGameSceneNodeExtended* pawn, QAngle_t* angViewAngles, int nPlayerTickCount);
        volatile ComputeRandomSeedFn original = (ComputeRandomSeedFn)g_pOriginalComputeRandomSeed;

        data->m_nRandomSeed = original((CS2::client::CGameSceneNodeExtended*)va1, (QAngle_t*)va2, (int)va3);
        return data->m_nRandomSeed;
    }

    void ComputeRandomSeed::hkComputeRandomSeedEnd() {}

    LH_END()

        uint32_t ComputeRandomSeed::GetRandomSeed() {
        if (!m_Hook.IsHooked())
            return -1;

        return m_Hook.ReadData<ComputeRandomSeedHookData>().m_nRandomSeed;
    }


    bool ComputeRandomSeed::Hook() {
        ComputeRandomSeedHookData initData{};
        initData.m_nRandomSeed = -1;

        return m_Hook.Hook<ComputeRandomSeedHookData>(
            COMPUTE_RANDOM_SEED_PATTERN,
            "client.dll",
            initData,
            reinterpret_cast<void*>(hkComputeRandomSeed),
            reinterpret_cast<void*>(hkComputeRandomSeedEnd),
            {
                LiquidHookEx::Detour::RipSlot::Data(&g_pComputeRandomSeedHookData),
                LiquidHookEx::Detour::RipSlot::Orig(&g_pOriginalComputeRandomSeed),
            },
            17
            );
    }

    bool ComputeRandomSeed::Unhook() {
        if (!m_Hook.IsHooked())
            return true;
        return m_Hook.Unhook();
    }
}