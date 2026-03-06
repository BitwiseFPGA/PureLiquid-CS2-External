#include <CS2/Hooks/Client/GetInaccuracy.h>
#include <CS2/Patterns.h>
#include <CS2/Offsets/client/CCSWeaponBaseVData.hpp>
#include <CS2/Offsets/client/C_BaseEntity.hpp>
#include <GlobalData/Include.h>
using namespace Globals;

namespace CS2 {

    static GetInaccuracyFnHookData* g_pGetSpreadHookData = nullptr;

#pragma code_seg(".getSpreadHook")
#pragma optimize("", off)
#pragma runtime_checks("", off)
#pragma check_stack(off)

    __declspec(noinline)
        float __fastcall GetInaccuracy::GetInaccuracy_Hook_Shellcode(
            client::C_CSWeaponBaseGun* a1, void* a2, void* a3)
    {
        uintptr_t vftable = *reinterpret_cast<uintptr_t*>(a1);
        typedef float(__fastcall* GetInaccuracyFn)(client::C_CSWeaponBaseGun*, void*, void*);
        GetInaccuracyFn original = *reinterpret_cast<GetInaccuracyFn*>(vftable + 0xCD0);

        float res = original(a1, a2, a3);

        volatile GetInaccuracyFnHookData* data = g_pGetSpreadHookData;
        data->flInaccuracy = res;
        data->weapon = a1;
        return res;
    }

    void GetInaccuracy::GetInaccuracy_Hook_Shellcode_End() {}

#pragma check_stack()
#pragma runtime_checks("", restore)
#pragma optimize("", on)
#pragma code_seg()

    bool GetInaccuracy::Hook()
    {
        GetInaccuracyFnHookData initData{};
        initData.weapon = nullptr;
        initData.flInaccuracy = 0.0f;

        return m_Hook.Hook<GetInaccuracyFnHookData>(
            GET_SPREAD_CALLSITE_PATTERN,
            "client.dll",
            initData,
            reinterpret_cast<void*>(GetInaccuracy_Hook_Shellcode),
            reinterpret_cast<void*>(GetInaccuracy_Hook_Shellcode_End),
            {
                LiquidCallHookEx::RipSlot::Data(&g_pGetSpreadHookData),
            });
    }

    bool GetInaccuracy::Unhook()
    {
        return m_Hook.Unhook();
    }

    GetInaccuracyFnHookData GetInaccuracy::GetData()
    {
        if (!m_Hook.IsHooked())
            return {};

        return m_Hook.ReadData<GetInaccuracyFnHookData>();
    }

} // namespace CS2