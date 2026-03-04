#include <CS2/Interfaces/CCSGOInput.h>
#include <CS2/Patterns.h>
#include <Math/Vector.h>
#include <GlobalData/Include.h>
#include <Memory/LiquidHookEx.h>
using namespace Globals;

namespace CS2 {
    static void* g_pOriginalCreateMove = nullptr;
    static CreateMoveHookData* g_pHookData = nullptr;

#pragma code_seg(".createMoveHook")
#pragma optimize("", off)
#pragma runtime_checks("", off)
#pragma check_stack(off)  

    double __fastcall CCSGOInput::CreateMove_Hook_Shellcode(
        int64_t a1, unsigned int a2, CUserCmd* cmd)
    {
        CreateMoveHookData* data = g_pHookData;
        if (!data) {
            typedef double(__fastcall* CreateMoveFn)(int64_t, unsigned int, CUserCmd*);
            CreateMoveFn original = (CreateMoveFn)g_pOriginalCreateMove;
            return original(a1, a2, cmd);
        }


        if (data->bForceSubtickViewAngle) {
            data->bForceSubtickViewAngle = false;
            data->cmd = (uint64_t)cmd;

            /*auto pHistoryBase = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(cmd) + 0x38);
            if (pHistoryBase) {
                auto pHistoryCountBase = *reinterpret_cast<uintptr_t*>(pHistoryBase);
                if (pHistoryCountBase) {
                    int pHistoryCount = *reinterpret_cast<int*>(pHistoryCountBase + +0x4);
                    if (pHistoryCount) {

                        for (int i = 1; i < pHistoryCount; i++) {
                            CSGOInputHistoryEntryPB* pHistoryEntry = *reinterpret_cast<CSGOInputHistoryEntryPB**>(pHistoryBase + (0x8 * i));
                            if (pHistoryEntry) {
                                if (pHistoryEntry->pViewCmd) {

                                    pHistoryEntry->pViewCmd->angValue.x = data->vViewAnglesToSet.x;
                                    pHistoryEntry->pViewCmd->angValue.y = data->vViewAnglesToSet.y;
                                    pHistoryEntry->pViewCmd->angValue.z = data->vViewAnglesToSet.z;
                                    // pHistoryEntry->has_bits |= INPUT_HISTORY_BITS_VIEWANGLES;
                                }
                            }
                        }
                    }
                }
                 }
                 */

            if (cmd->csgoUserCmd.pBaseCmd && cmd->csgoUserCmd.pBaseCmd->pViewAngles) {
                cmd->csgoUserCmd.pBaseCmd->pViewAngles->angValue.x = data->vViewAnglesToSet.x;
                cmd->csgoUserCmd.pBaseCmd->pViewAngles->angValue.y = data->vViewAnglesToSet.y;
                cmd->csgoUserCmd.pBaseCmd->pViewAngles->angValue.z = data->vViewAnglesToSet.z;
            }

        }

        if (data->bForceBtn) {
            data->bForcedBtn = false;
            cmd->nButtons.nValue |= data->btnToForce;
            data->bForceBtn = false;
            data->btnToForce = 0;
            data->bForcedBtn = true;
        }

        uintptr_t pBase = *(uintptr_t*)((uintptr_t)cmd + 0x40);

        if (pBase) {
            data->m_nRandomSeed = *(int*)(pBase + 0x6C);
        }

        // Call original
        typedef double(__fastcall* CreateMoveFn)(int64_t, unsigned int, CUserCmd*);
        CreateMoveFn original = (CreateMoveFn)g_pOriginalCreateMove;
        return original(a1, a2, cmd);
    }

    void CCSGOInput::CreateMove_Hook_Shellcode_End() {}

#pragma check_stack()  
#pragma runtime_checks("", restore) 
#pragma optimize("", on)
#pragma code_seg()

    uintptr_t CCSGOInput::FindCreateMove()
    {
        auto client = proc.GetRemoteModule("client.dll");
        if (!client || !client->IsValid()) {
            printf("[!] Failed to get client.dll\n");
            return 0;
        }

        // Pattern for CreateMove function
        const char* pattern = "48 8B C4 4C 89 40 ?? 48 89 48 ?? 55 53 41 54";

        uint8_t* addr = client->ScanMemory(pattern);
        if (!addr) {
            printf("[!] Failed to find CreateMove pattern\n");
            return 0;
        }

        uintptr_t createMoveAddr = reinterpret_cast<uintptr_t>(addr);
        printf("[+] Found CreateMove at: client.dll + 0x%llX\n",
            createMoveAddr - client->GetAddr());

        return createMoveAddr;
    }



    // -----------------------------------------------------------------------
    // TryRestore – reload hook state saved by a previous process instance.
    // Returns true if the state was valid and all members were restored,
    // so that HookCreateMove() can skip re-injecting shellcode.
    // -----------------------------------------------------------------------
    bool CCSGOInput::TryRestore()
    {
        return true;
    }

    bool CCSGOInput::HookCreateMove()
    {

        CreateMoveHookData initData{};
        initData.bForceBtn = false;
        initData.bForcedBtn = true;
        initData.btnToForce = IN_JUMP;
        initData.bForceSubtickViewAngle = false;
        initData.vViewAnglesToSet = { 0.f, 0.f, 0.f };


        return m_Hook.Hook<CreateMoveHookData>(
            "48 8B C4 4C 89 40 ?? 48 89 48 ?? 55 53 41 54",
            "client.dll",
            initData,
            reinterpret_cast<void*>(CreateMove_Hook_Shellcode),
            reinterpret_cast<void*>(CreateMove_Hook_Shellcode_End),
            {
                LiquidHookEx::RipSlot::Data(&g_pHookData),
                LiquidHookEx::RipSlot::Orig(&g_pOriginalCreateMove),
            }
            );
    }

    bool CCSGOInput::UnhookCreateMove()
    {
        return m_Hook.Unhook();
    }

    void CCSGOInput::ForceButton(ECommandButtons btn)
    {

        if (!m_Hook.IsHooked())
            return;

        m_Hook.WriteField<bool>(offsetof(CreateMoveHookData, bForceBtn), true);

        m_Hook.WriteField<bool>(offsetof(CreateMoveHookData, bForceBtn), true);

        m_Hook.WriteField<uint64_t>(offsetof(CreateMoveHookData, btnToForce), btn);

    }

    void CCSGOInput::Attack() {
        ForceButton(IN_ATTACK);
    }


    void CCSGOInput::SetSubTickAngle(Vector vAngle) {

        if (!m_Hook.IsHooked()) {
            vViewAngles = QAngle_t{ vAngle.x,vAngle.y,vAngle.z };
            return;
        }
        m_Hook.WriteField<Vector>(offsetof(CreateMoveHookData, vViewAnglesToSet), vAngle);
        m_Hook.WriteField<bool>(offsetof(CreateMoveHookData, bForceSubtickViewAngle), true);

    }

    CreateMoveHookData CCSGOInput::GetExecutionData()
    {
        return m_Hook.ReadData<CreateMoveHookData>();
    }

    int CCSGOInput::GetRandomSeed() {
        return GetExecutionData().m_nRandomSeed;
    }

    uintptr_t CCSGOInput::ResolveViewAnglesOffset() {
        if (!pViewAnglesOffset) {
            auto hClient = ::Globals::proc.GetRemoteModule("client.dll");
            if (!hClient || hClient && !hClient->GetAddr()) {
                return NULL;
            }
            auto pViewAngleOffsetDisp = hClient->ScanMemory(C_CSGO_INPUT_VIEW_ANGLES_PATTERN);
            pViewAnglesOffset = hClient->ResolveDisp32(pViewAngleOffsetDisp, 3);
            if (!pViewAnglesOffset)
                return NULL;
        }

        return pViewAnglesOffset;
    }

}