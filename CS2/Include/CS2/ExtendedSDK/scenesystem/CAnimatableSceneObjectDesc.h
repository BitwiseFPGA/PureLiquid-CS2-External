#pragma once
#include <cstdint>
#include <array>
struct VTableFunctionInfo;
namespace Source2 {
    template <typename T>
    class CStrongHandle;
}
namespace CS2 {


    constexpr size_t MAX_VISIBLE_PLAYERS = 64;

    using CMaterial2 = void;
    class CBaseSceneData;

    struct CAnimatableSceneObjectDescRenderHookData
    {
        uint64_t originalFunc;
        ::Source2::CStrongHandle<CMaterial2>* hMaterialToUse;
        bool bChamsEnabled;

        // Default chams color
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;

        // Visible chams color
        uint8_t r_visible;
        uint8_t g_visible;
        uint8_t b_visible;
        uint8_t a_visible;

        char weaponStr[8];
        uintptr_t pStrstr;

        uint32_t mVisiblePawnIndexes[MAX_VISIBLE_PLAYERS];
    };

    class CAnimatableSceneObjectDesc
    {
    private:
        inline static bool m_bIsHooked = false;
        inline static void* m_pDataRemote = nullptr;
        inline static void* m_pShellcodeRemote = nullptr;
        inline static uintptr_t m_pTargetFunction = 0;

        static void* __fastcall RenderObjects_Hook_Shellcode(
            uint64_t a1, uint64_t a2, CBaseSceneData* a3, int32_t a4,
            uint64_t a5, uint64_t a6, uint64_t a7);
        static void RenderObjects_Hook_Shellcode_End();

        static uintptr_t FindRendererFn();

        static VTableFunctionInfo FindVTableDataForRendererFunction();
    public:

        static bool InstallRendererHook(::Source2::CStrongHandle<CMaterial2>* hMaterialToUse);

        static bool UninstallRendererHook();

        static CAnimatableSceneObjectDescRenderHookData GetExecutionData();
        static void SetChamsEnabled(bool bActive);
        static void SetChamsMaterial(::Source2::CStrongHandle<CMaterial2>* mat);
        static void SetChamsColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool bLog = false);

        static void SetVisibleChamsColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool bLog = false);

        static void UpdateVisiblePawnIndexes(const uint32_t* indexes, size_t count);


        static bool IsHooked() { return m_bIsHooked; }
    };

}