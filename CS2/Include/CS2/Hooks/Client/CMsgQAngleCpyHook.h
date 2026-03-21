#pragma once
#include <Math/QAngle.h>
#include <Math/Vector.h>
#include <LiquidHookEx/Include.h>
#include <GlobalData/Include.h>

namespace CS2 {

    class CMsgQAngleTest
    {
    public:
        class CBasePB* base; //0x00
        char pad_0008[16]; //0x08
        QAngle angValue; //0x18
        char pad_0024[92]; //0x24
    }; //Size: 0x0080

    struct CMsgQAngleCpyHookData : public LiquidHookEx::CallSite::BaseHookData
    {
        Vector qAngleToSet;
        bool   bForceQAngle;
    };

    class CMsgQAngleCpy {
    private:
        inline static LiquidHookEx::CallSite m_Hook = LiquidHookEx::CallSite("CMsgQAngleCpyHook");

        static void __fastcall SetAng_Hook_Shellcode(uintptr_t a1, CMsgQAngleTest* a2);
        static void SetAng_Hook_Shellcode_End();

    public:
        static bool Hook();
        static bool Unhook();
        static void ToggleAngleChange();
        static void SetAngleChange(bool bEnabled);
        static void SetAngle(Vector vAngle);
        static bool IsHooked() { return m_Hook.IsHooked(); }
    };

} // namespace CS2