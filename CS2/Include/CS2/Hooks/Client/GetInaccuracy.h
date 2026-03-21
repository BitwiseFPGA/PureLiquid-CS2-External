#pragma once
#include <GlobalData/Include.h>
#include <LiquidHookEx/Include.h>

namespace CS2 {
    namespace client {
        class C_CSWeaponBaseGun;
    }

    struct GetInaccuracyFnHookData : public LiquidHookEx::CallSite::BaseHookData
    {
        client::C_CSWeaponBaseGun* weapon;
        float flInaccuracy;
    };

    class GetInaccuracy {
    private:
        inline static LiquidHookEx::CallSite m_Hook = LiquidHookEx::CallSite("GetInaccuracyHook");

        static float __fastcall GetInaccuracy_Hook_Shellcode(
            client::C_CSWeaponBaseGun* pWeapon, void* unknown, void* unknown2);
        static void GetInaccuracy_Hook_Shellcode_End();

    public:
        static bool Hook();
        static bool Unhook();
        static GetInaccuracyFnHookData GetData();
        static bool IsHooked() { return m_Hook.IsHooked(); }
    };

} // namespace CS2