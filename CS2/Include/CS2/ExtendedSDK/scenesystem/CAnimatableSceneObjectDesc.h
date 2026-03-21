#pragma once
#include <cstdint>
#include <array>
#include <LiquidHookEx/Include.h>
#include <GlobalData/Include.h>

namespace Source2 {
    template <typename T>
    class CStrongHandle;
}

namespace CS2 {


    constexpr size_t MAX_VISIBLE_PLAYERS = 64;

    using CMaterial2 = void;
    class CBaseSceneData;

    struct CAnimatableSceneObjectDescRenderHookData : public LiquidHookEx::VTable::BaseHookData
    {
        ::Source2::CStrongHandle<CMaterial2>* hMaterialToUse;
        bool bChamsEnabled;

        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;

        char weaponStr[8];
        uintptr_t pStrstr;
    };

    class CAnimatableSceneObjectDesc
    {
    private:
        inline static LiquidHookEx::VTable m_Hook = LiquidHookEx::VTable("CAnimatableSceneObjectDescHook");

        static void* __fastcall RenderObjects_Hook_Shellcode(
            uint64_t a1, uint64_t a2, CBaseSceneData* a3, int32_t a4,
            uint64_t a5, uint64_t a6, uint64_t a7);
        static void RenderObjects_Hook_Shellcode_End();
    public:

        static bool InstallRendererHook(::Source2::CStrongHandle<CMaterial2>* hMaterialToUse);

        static bool UninstallRendererHook();
        static CAnimatableSceneObjectDescRenderHookData GetExecutionData();
        static void SetChamsEnabled(bool bActive);
        static void SetChamsMaterial(::Source2::CStrongHandle<CMaterial2>* mat);
        static void SetChamsColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool bLog = false);

        static bool IsHooked() { return m_Hook.IsHooked(); }
    };

}