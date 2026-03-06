#include <CS2/Hooks/Client/CMsgQAngleCpyHook.h>
#include <CS2/Patterns.h>
#include <GlobalData/Include.h>
using namespace Globals;

namespace CS2 {

    static void* g_pOriginalSetAng = nullptr;
    static CMsgQAngleCpyHookData* g_pSetAngHookData = nullptr;

#pragma code_seg(".msgQangleCpyHook")
#pragma optimize("", off)
#pragma runtime_checks("", off)
#pragma check_stack(off)

    __declspec(noinline)
        void __fastcall CMsgQAngleCpy::SetAng_Hook_Shellcode(
            uintptr_t a1, CMsgQAngleTest* a2)
    {
        volatile uintptr_t         va1 = a1;
        volatile CMsgQAngleTest* va2 = a2;

        volatile CMsgQAngleCpyHookData* data = g_pSetAngHookData;

        if (data && data->bForceQAngle) {
            if (va2) {
                va2->angValue.x = data->qAngleToSet.x;
                va2->angValue.y = data->qAngleToSet.y;
                va2->angValue.z = data->qAngleToSet.z;
            }
        }

        typedef void(__fastcall* SetAngFn)(uintptr_t, CMsgQAngleTest*);
        volatile SetAngFn original = (SetAngFn)g_pOriginalSetAng;

        return original((uintptr_t)va1, (CMsgQAngleTest*)va2);
    }

    void CMsgQAngleCpy::SetAng_Hook_Shellcode_End() {}

#pragma check_stack()
#pragma runtime_checks("", restore)
#pragma optimize("", on)
#pragma code_seg()

    bool CMsgQAngleCpy::Hook()
    {
        CMsgQAngleCpyHookData initData{};
        initData.bForceQAngle = false;
        initData.qAngleToSet = { 0.0f, 0.0f, 0.0f };

        // The call site is a 5-byte E8 followed by a 4-byte "test sil, 2"
        // (40 F6 C6 02) that must also be NOP'd out for correct execution flow.
        // Total region = 9 bytes.
        return m_Hook.Hook<CMsgQAngleCpyHookData>(
            SET_SERVER_ANGLE_CALLSITE_PATTERN,
            "client.dll",
            initData,
            reinterpret_cast<void*>(SetAng_Hook_Shellcode),
            reinterpret_cast<void*>(SetAng_Hook_Shellcode_End),
            {
                LiquidMidHookEx::RipSlot::Data(&g_pSetAngHookData),
                LiquidMidHookEx::RipSlot::Orig(&g_pOriginalSetAng),
            },
            9);
    }

    bool CMsgQAngleCpy::Unhook()
    {
        return m_Hook.Unhook();
    }


    void CMsgQAngleCpy::ToggleAngleChange()
    {
        if (!m_Hook.IsHooked()) return;

        auto data = m_Hook.ReadData<CMsgQAngleCpyHookData>();
        bool current = data.bForceQAngle;
        m_Hook.WriteField<bool>(offsetof(CMsgQAngleCpyHookData, bForceQAngle), !current);
    }

    void CMsgQAngleCpy::SetAngleChange(bool bEnabled)
    {
        if (!m_Hook.IsHooked()) return;
        m_Hook.WriteField<bool>(offsetof(CMsgQAngleCpyHookData, bForceQAngle), bEnabled);
    }

    void CMsgQAngleCpy::SetAngle(Vector vAngle)
    {
        if (!m_Hook.IsHooked()) return;
        m_Hook.WriteField<Vector>(offsetof(CMsgQAngleCpyHookData, qAngleToSet), vAngle);
    }

} // namespace CS2