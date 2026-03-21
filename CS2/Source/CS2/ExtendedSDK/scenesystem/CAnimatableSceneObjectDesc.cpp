#include <CS2/ExtendedSDK/scenesystem/CAnimatableSceneObjectDesc.h>
#include <CS2/ExtendedSDK/scenesystem/CBaseSceneData.h>
#include <CS2/Patterns.h>
#include <Source2/CStrongHandle.h>
#include <GlobalData/Include.h>
#include <string>
#include <array>
using namespace Globals;



namespace CS2 {

    static void* g_pOriginalRenderObjects = nullptr;
    static CAnimatableSceneObjectDescRenderHookData* g_pHookData = nullptr;

#pragma code_seg(".CAnimatableSceneObjectDescRenderHookSection")
#pragma optimize("", off)

    void* __fastcall CAnimatableSceneObjectDesc::RenderObjects_Hook_Shellcode(
        uint64_t a1, uint64_t a2, CBaseSceneData* a3, int32_t a4,
        uint64_t a5, uint64_t a6, uint64_t a7)
    {
        CAnimatableSceneObjectDescRenderHookData* data = g_pHookData;

        typedef void* (__fastcall* RenderObjectsFn)(uint64_t, uint64_t, CBaseSceneData*, int32_t, uint64_t, uint64_t, uint64_t);
        RenderObjectsFn original = (RenderObjectsFn)g_pOriginalRenderObjects;

        if (!data) {
            return original(a1, a2, a3, a4, a5, a6, a7);
        }


        auto pModelImpl = *reinterpret_cast<uintptr_t*>(
            *reinterpret_cast<uintptr_t*>(
                *reinterpret_cast<uintptr_t*>(
                    reinterpret_cast<uintptr_t>(a3)
                    ) + 0x8
                ));

        auto pModelStr = *reinterpret_cast<const char**>(pModelImpl + 0x8);

        typedef char* (__cdecl* StrstrFn)(const char*, const char*);
        StrstrFn strstr_fn = (StrstrFn)data->pStrstr;

        if (strstr_fn(pModelStr, data->weaponStr)) {
            return original(a1, a2, a3, a4, a5, a6, a7);
        }




        if (data->bChamsEnabled) {

            bool isVisible = false;

            /*
            uint32_t handleValue = *reinterpret_cast<uint32_t*>(&a3->sceneObject->m_hOwner);
            if (handleValue) {
                uint32_t entityIndex = handleValue & 0x7FFF;
                for (size_t i = 0; i < MAX_VISIBLE_PLAYERS; i++) {
                    uint32_t idx = data->mVisiblePawnIndexes[i];
                    // if (idx == 0) break;
                    if (idx == entityIndex) {
                        isVisible = true;
                        break;
                    }
                }
            }*/
            for (int i = 0; i < a4; ++i)
            {
                auto scene = &a3[i];
                if (scene) {
                    scene->r =  data->r;
                    scene->g =  data->g;
                    scene->b = data->b;
                    scene->a = data->a;
                    if (data->hMaterialToUse) {
                        scene->material = data->hMaterialToUse->pData;
                        scene->material2 = data->hMaterialToUse->pData;
                    }
                }
            }
        }

        return original(a1, a2, a3, a4, a5, a6, a7);
    }

    void CAnimatableSceneObjectDesc::RenderObjects_Hook_Shellcode_End() {}

#pragma optimize("", on)
#pragma code_seg()

    bool CAnimatableSceneObjectDesc::InstallRendererHook(::Source2::CStrongHandle<CMaterial2>* hMaterialToUse)
    {

        CAnimatableSceneObjectDescRenderHookData data{};
        data.hMaterialToUse = hMaterialToUse;
        data.bChamsEnabled = false;

        data.r = 255;
        data.g = 255;
        data.b = 255;
        data.a = 25;

        data.pStrstr = reinterpret_cast<uintptr_t>(GetProcAddress(GetModuleHandleA("ucrtbase.dll"), "strstr"));

        strcpy_s(data.weaponStr, sizeof(data.weaponStr), "weapon");

        return m_Hook.Hook<CAnimatableSceneObjectDescRenderHookData>(
            CANIMATABLE_SCENE_OBJECT_DESC_RENDER_FN_PATTERN,
            "scenesystem.dll",
            data,
            reinterpret_cast<void*>(RenderObjects_Hook_Shellcode),
            reinterpret_cast<void*>(RenderObjects_Hook_Shellcode_End),
            {
                LiquidHookEx::VTable::RipSlot::Data(&g_pHookData),
                LiquidHookEx::VTable::RipSlot::Orig(&g_pOriginalRenderObjects),
            }
            );
  
    }

    bool CAnimatableSceneObjectDesc::UninstallRendererHook()
    {
        return m_Hook.Unhook();
    }

    CAnimatableSceneObjectDescRenderHookData CAnimatableSceneObjectDesc::GetExecutionData()
    {
        return m_Hook.ReadData<CAnimatableSceneObjectDescRenderHookData>();
    }

    void CAnimatableSceneObjectDesc::SetChamsEnabled(bool bActive) {

        if (!m_Hook.IsHooked())
            return;

        m_Hook.WriteField<bool>(offsetof(CAnimatableSceneObjectDescRenderHookData, bChamsEnabled), bActive);
    }

    void CAnimatableSceneObjectDesc::SetChamsColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool bLog) {

        if (!m_Hook.IsHooked())
            return;

        m_Hook.WriteField<uint8_t>(offsetof(CAnimatableSceneObjectDescRenderHookData, r), r);
        m_Hook.WriteField<uint8_t>(offsetof(CAnimatableSceneObjectDescRenderHookData, g), g);
        m_Hook.WriteField<uint8_t>(offsetof(CAnimatableSceneObjectDescRenderHookData, b), b);
        m_Hook.WriteField<uint8_t>(offsetof(CAnimatableSceneObjectDescRenderHookData, a), a);
    }

    void CAnimatableSceneObjectDesc::SetChamsMaterial(::Source2::CStrongHandle<CMaterial2>* mat) {
        if (!m_Hook.IsHooked())
            return;

        m_Hook.WriteField<::Source2::CStrongHandle<CMaterial2>*>(offsetof(CAnimatableSceneObjectDescRenderHookData, hMaterialToUse), mat);
    }
}