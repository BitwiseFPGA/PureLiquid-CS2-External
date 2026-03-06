#pragma once
#include <GlobalData/Include.h>
#include <Memory/LiquidCallHookEx.h>

namespace CS2 {
    namespace client {
        class C_CSWeaponBaseGun;
    }

    struct GetInaccuracyFnHookData : public LiquidCallHookEx::BaseHookData
    {
        client::C_CSWeaponBaseGun* weapon;
        float flInaccuracy;
    };

    class GetInaccuracy {
    private:
        inline static LiquidCallHookEx m_Hook = LiquidCallHookEx("GetInaccuracyHook", &Globals::proc);

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