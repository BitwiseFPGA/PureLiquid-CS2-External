#include <CS2/Hooks/Client/CMsgQAngleCpyHook.h>
#include <GlobalData/Include.h>
using namespace Globals;
#include <Windows.h>

namespace CS2 {


    // subtick movestep
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
        volatile uintptr_t va1 = a1;
        volatile CMsgQAngleTest *va2 = a2;



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
        auto client = proc.GetRemoteModule("client.dll");
        if (!client || !client->IsValid()) {
            printf("[!] Failed to get client.dll\n");
            return false;
        }

        auto pSetAngFnCall = client->ScanMemory("E8 ?? ?? ?? ?? 40 F6 C6 ?? 74 ?? 83 0F ?? 48 8B 43");
        auto pSetAngFn = client->ResolveInstruction(reinterpret_cast<uintptr_t>(pSetAngFnCall), 0, true);

        if (!pSetAngFn) {
            printf("Failed to get CMsgQAngleCpy::SetAng function!\n");
            return false;
        }

        printf("[+] CMsgQAngleCpy::SetAng  address: client.dll + 0x%llX\n", pSetAngFn - client->GetAddr());

        g_pOriginalSetAng = reinterpret_cast<void*>(pSetAngFn);

        m_pDataRemote = proc.Alloc(sizeof(CMsgQAngleCpyHookData));
        if (!m_pDataRemote) {
            printf("[!] Failed to allocate CMsgQAngleCpyHookData\n");
            return false;
        }
        printf("[+] Remote CMsgQAngleCpyHookData: 0x%p\n", m_pDataRemote);

        CMsgQAngleCpyHookData data{};
        data.pOriginal = reinterpret_cast<OriginalCpyQAngleFnDef>(g_pOriginalSetAng);
        data.bForceQAngle = false;
        data.qAngleToSet = { 0.0f, 0.0f, 0.0f };

        proc.Write<CMsgQAngleCpyHookData>(reinterpret_cast<uintptr_t>(m_pDataRemote), data);

        size_t shellcodeSize = reinterpret_cast<uintptr_t>(SetAng_Hook_Shellcode_End) -
            reinterpret_cast<uintptr_t>(SetAng_Hook_Shellcode);

        m_pShellcodeRemote = proc.Alloc(shellcodeSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (!m_pShellcodeRemote) {
            printf("[!] Failed to allocate shellcode memory\n");
            return false;
        }

        std::vector<uint8_t> localCode(shellcodeSize);
        memcpy(localCode.data(), SetAng_Hook_Shellcode, shellcodeSize);

        if (!proc.WriteArray(reinterpret_cast<uintptr_t>(m_pShellcodeRemote), localCode)) {
            printf("[!] Failed to write shellcode\n");
            return false;
        }

        printf("[+] Remote shellcode: 0x%p (size: %zu bytes)\n", m_pShellcodeRemote, shellcodeSize);

        // Allocate storage for g_pSetAngHookData pointer
        void* pHookDataStorage = proc.Alloc(8);
        if (!pHookDataStorage) {
            printf("[!] Failed to allocate hook data storage\n");
            return false;
        }

        if (!proc.Write<uint64_t>(reinterpret_cast<uintptr_t>(pHookDataStorage),
            reinterpret_cast<uint64_t>(m_pDataRemote))) {
            printf("[!] Failed to write hook data pointer\n");
            return false;
        }

        printf("[+] g_pSetAngHookData storage: 0x%p -> 0x%p\n", pHookDataStorage, m_pDataRemote);

        // Allocate storage for g_pOriginalSetAng pointer
        void* pOrigPtrStorage = proc.Alloc(8);
        if (!pOrigPtrStorage) {
            printf("[!] Failed to allocate orig pointer storage\n");
            return false;
        }

        if (!proc.Write<uint64_t>(reinterpret_cast<uintptr_t>(pOrigPtrStorage), reinterpret_cast<uint64_t>(g_pOriginalSetAng))) {
            printf("[!] Failed to write original pointer\n");
            return false;
        }

        printf("[+] g_pOriginalSetAng storage: 0x%p -> 0x%llX\n", pOrigPtrStorage, g_pOriginalSetAng);

        // ================================================================
        // Patch RIP-relative instructions for BOTH globals
        // ================================================================
        printf("[*] Patching RIP-relative instructions...\n");

        int ripLoadCount = 0;
        int patchedCount = 0;

        for (size_t i = 0; i < shellcodeSize - 7; i++) {
            // Look for: mov reg, [rip+offset] (48 8B 05 ?? ?? ?? ??)
            if (localCode[i] == 0x48 && localCode[i + 1] == 0x8B && localCode[i + 2] == 0x05) {
                ripLoadCount++;

                uintptr_t instructionAddr = reinterpret_cast<uintptr_t>(m_pShellcodeRemote) + i;
                uintptr_t targetAddr;
                const char* varName;

                // First RIP load is g_pSetAngHookData (the data pointer)
                if (ripLoadCount == 1) {
                    targetAddr = reinterpret_cast<uintptr_t>(pHookDataStorage);
                    varName = "g_pSetAngHookData";
                }
                // Second and subsequent RIP loads are g_pOriginalSetAng (the function pointer)
                else {
                    targetAddr = reinterpret_cast<uintptr_t>(pOrigPtrStorage);
                    varName = "g_pOriginalSetAng";
                }

                int32_t newOffset = static_cast<int32_t>(targetAddr - (instructionAddr + 7));

                if (!proc.Write<int32_t>(instructionAddr + 3, newOffset)) {
                    printf("[!] Failed to patch RIP offset at 0x%zX\n", i);
                    return false;
                }

                printf("[+] Patched RIP-relative %s at offset 0x%zX (target: 0x%p, offset: 0x%X)\n",
                    varName, i, (void*)targetAddr, newOffset);
                patchedCount++;
            }
        }

        if (patchedCount == 0) {
            printf("[!] WARNING: No RIP-relative instructions found! Shellcode may not work.\n");
        }
        else if (patchedCount < 2) {
            printf("[!] WARNING: Only found %d RIP load(s), expected at least 2!\n", patchedCount);
        }
        else {
            printf("[+] Successfully patched %d RIP-relative instruction(s)\n", patchedCount);
        }

        if (!FlushInstructionCache(proc.m_hProc, m_pShellcodeRemote, shellcodeSize)) {
            printf("[!] Warning: FlushInstructionCache failed: %d\n", GetLastError());
        }
        // @client.dll
        /*
        
        .rdata:00000000015E4990                         ; const CSGOInterpolationInfoPB_CL::`vftable'
.rdata:00000000015E4990 F0 17 54 00 00 00 00 00 ??_7CSGOInterpolationInfoPB_CL@@6B@ dq offset sub_5417F0
.rdata:00000000015E4990                                                                 ; DATA XREF: sub_5417F0+F↑o
.rdata:00000000015E4990                                                                 ; sub_543CC0+15↑o ...
.rdata:00000000015E4998 A0 00 E1 00 00 00 00 00                 dq offset sub_E100A0
.rdata:00000000015E49A0 40 15 54 00 00 00 00 00                 dq offset sub_541540
.rdata:00000000015E49A8 80 15 54 00 00 00 00 00                 dq offset sub_541580
.rdata:00000000015E49B0 A0 15 54 00 00 00 00 00                 dq offset sub_5415A0
.rdata:00000000015E49B8 D0 00 E1 00 00 00 00 00                 dq offset sub_E100D0
.rdata:00000000015E49C0 00 F5 E0 00 00 00 00 00                 dq offset sub_E0F500
.rdata:00000000015E49C8 B0 15 54 00 00 00 00 00                 dq offset sub_5415B0
.rdata:00000000015E49D0 80 17 54 00 00 00 00 00                 dq offset sub_541780
.rdata:00000000015E49D8 D0 15 54 00 00 00 00 00                 dq offset sub_5415D0
.rdata:00000000015E49E0 50 B4 13 00 00 00 00 00                 dq offset nullsub_1385
.rdata:00000000015E49E8 10 17 54 00 00 00 00 00                 dq offset sub_541710
.rdata:00000000015E49F0 F0 05 E1 00 00 00 00 00                 dq offset sub_E105F0
.rdata:00000000015E49F8 10 0D E1 00 00 00 00 00                 dq offset sub_E10D10
.rdata:00000000015E4A00 90 17 54 00 00 00 00 00                 dq offset sub_541790
.rdata:00000000015E4A08 B0 17 54 00 00 00 00 00                 dq offset sub_5417B0
.rdata:00000000015E4A10 A0 17 54 00 00 00 00 00                 dq offset sub_5417A0
.rdata:00000000015E4A18 C0 F8 E0 00 00 00 00 00 off_15E4A18     dq offset sub_E0F8C0    ; DATA XREF: sub_542CE0↑o
.rdata:00000000015E4A20 90 18 54 00 00 00 00 00                 dq offset SetServerAngle_Interpolation_sub_541890 <= This function

SetServerAngle_Interpolation_sub_541890 + 0x7E
.text:000000000054190E E8 FD 44 F3 FF                          call    SetServerAngle_sub_475E10 <= Patch This call!!



Also found in VTable:
data:00000000015AE520                         ; const CUserMessage_Diagnostic_Response_Diagnostic::`vftable'
.rdata:00000000015AE520 60 61 47 00 00 00 00 00 ??_7CUserMessage_Diagnostic_Response_Diagnostic@@6B@ dq offset sub_476160
.rdata:00000000015AE520                                                                 ; DATA XREF: sub_476160+F↑o
.rdata:00000000015AE520                                                                 ; sub_4CAD60+51↑o ...
.rdata:00000000015AE528 A0 00 E1 00 00 00 00 00                 dq offset sub_E100A0
.rdata:00000000015AE530 10 21 47 00 00 00 00 00                 dq offset sub_472110
.rdata:00000000015AE538 F0 22 47 00 00 00 00 00                 dq offset sub_4722F0
.rdata:00000000015AE540 B0 23 47 00 00 00 00 00                 dq offset sub_4723B0
.rdata:00000000015AE548 D0 00 E1 00 00 00 00 00                 dq offset sub_E100D0
.rdata:00000000015AE550 00 F5 E0 00 00 00 00 00                 dq offset sub_E0F500
.rdata:00000000015AE558 D0 25 47 00 00 00 00 00                 dq offset sub_4725D0
.rdata:00000000015AE560 B0 3B 47 00 00 00 00 00                 dq offset sub_473BB0
.rdata:00000000015AE568 60 2C 47 00 00 00 00 00                 dq offset sub_472C60
.rdata:00000000015AE570 50 B4 13 00 00 00 00 00                 dq offset nullsub_1385
.rdata:00000000015AE578 60 36 47 00 00 00 00 00                 dq offset sub_473660
.rdata:00000000015AE580 F0 05 E1 00 00 00 00 00                 dq offset sub_E105F0
.rdata:00000000015AE588 10 0D E1 00 00 00 00 00                 dq offset sub_E10D10
.rdata:00000000015AE590 90 3D 47 00 00 00 00 00                 dq offset sub_473D90
.rdata:00000000015AE598 00 3F 47 00 00 00 00 00                 dq offset sub_473F00
.rdata:00000000015AE5A0 F0 3E 47 00 00 00 00 00                 dq offset sub_473EF0
.rdata:00000000015AE5A8 C0 F8 E0 00 00 00 00 00 off_15AE5A8     dq offset sub_E0F8C0    ; DATA XREF: sub_476220↑o
.rdata:00000000015AE5B0 10 5E 47 00 00 00 00 00                 dq offset SetServerAngle_sub_475E10 <= This
        */


        auto pCallSite = client->ScanMemory("E8 ?? ?? ?? ?? 40 F6 C6 ?? 74 ?? 83 0F ?? 48 8B 43");

        if (!pCallSite) {
            printf("[!] Couldn't find most common call!\n");
            return false;
        }

        printf("[+] Found call site at: client.dll + 0x%llX\n", pCallSite - client->GetAddr());

        uintptr_t callSiteAddr = reinterpret_cast<uintptr_t>(pCallSite);

        SYSTEM_INFO si;
        GetSystemInfo(&si);

        uintptr_t searchStart = (callSiteAddr > 0x7FFFFFFF) ?
            ((callSiteAddr - 0x7FFFFFFF) & ~(static_cast<uintptr_t>(si.dwAllocationGranularity) - 1)) : 0;
        uintptr_t searchEnd = callSiteAddr + 0x7FFFFFFF;

        void* pFuncPtrStorage = nullptr;

        for (uintptr_t addr = searchStart; addr < searchEnd && addr >= searchStart;
            addr += si.dwAllocationGranularity) {
            pFuncPtrStorage = VirtualAllocEx(proc.m_hProc, reinterpret_cast<void*>(addr),
                8, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            if (pFuncPtrStorage) {
                int64_t distance = reinterpret_cast<uintptr_t>(pFuncPtrStorage) - (callSiteAddr + 6);
                if (distance >= INT32_MIN && distance <= INT32_MAX) {
                    printf("[+] Allocated function pointer storage at: 0x%p (distance: 0x%llX)\n",
                        pFuncPtrStorage, distance);
                    break;
                }
                VirtualFreeEx(proc.m_hProc, pFuncPtrStorage, 0, MEM_RELEASE);
                pFuncPtrStorage = nullptr;
            }
        }

        if (!pFuncPtrStorage) {
            printf("[!] Failed to allocate function pointer storage within ±2GB of call site\n");
            return false;
        }

        uint64_t targetAddr = reinterpret_cast<uint64_t>(m_pShellcodeRemote);

        if (!proc.Write<uint64_t>(reinterpret_cast<uintptr_t>(pFuncPtrStorage), targetAddr)) {
            printf("[!] Failed to write function pointer\n");
            return false;
        }

        int64_t distance = reinterpret_cast<uintptr_t>(pFuncPtrStorage) - (callSiteAddr + 6);
        int32_t ripOffset = static_cast<int32_t>(distance);

        if (distance != ripOffset) {
            printf("[!] ERROR: RIP offset overflow! Distance: 0x%llX\n", distance);
            return false;
        }

        std::vector<uint8_t> newInstruction = {
            0xFF, 0x15,
            static_cast<uint8_t>(ripOffset & 0xFF),
            static_cast<uint8_t>((ripOffset >> 8) & 0xFF),
            static_cast<uint8_t>((ripOffset >> 16) & 0xFF),
            static_cast<uint8_t>((ripOffset >> 24) & 0xFF)
        };

        DWORD oldProtect2;
        if (!VirtualProtectEx(proc.m_hProc, reinterpret_cast<void*>(callSiteAddr),
            8, PAGE_EXECUTE_READWRITE, &oldProtect2)) {
            printf("[!] VirtualProtectEx failed: %d\n", GetLastError());
            return false;
        }

        if (!proc.WriteArray(callSiteAddr, newInstruction)) {
            printf("[!] Failed to patch call instruction\n");
            VirtualProtectEx(proc.m_hProc, reinterpret_cast<void*>(callSiteAddr),
                9, oldProtect2, &oldProtect2);
            return false;
        }

        // NOP out bytes 6, 7, and 8 (3 NOPs total)
        if (!proc.Write<uint8_t>(callSiteAddr + 6, 0x90)) {
            printf("[!] Failed to write first NOP\n");
        }
        if (!proc.Write<uint8_t>(callSiteAddr + 7, 0x90)) {
            printf("[!] Failed to write second NOP\n");
        }
        if (!proc.Write<uint8_t>(callSiteAddr + 8, 0x90)) {
            printf("[!] Failed to write third NOP\n");
        }
        else {
            printf("[+] NOPed 3 bytes at offset +6, +7, and +8\n");
        }

        VirtualProtectEx(proc.m_hProc, reinterpret_cast<void*>(callSiteAddr),
            9, oldProtect2, &oldProtect2);

        m_pPatchedMostCommonCallAddr = pCallSite;
        printf("[+] Successfully patched call site!\n\n");


        return true;
    }

    bool CMsgQAngleCpy::Unhook()
    {
        if (!m_bIsHooked) {
            printf("[!] CMsgQAngleCpy is not hooked\n");
            return false;
        }

        auto client = proc.GetRemoteModule("client.dll");
        if (!client || !client->IsValid()) {
            printf("[!] Failed to get client.dll for unhooking\n");
            return false;
        }

        bool success = true;

        // Restore the patched call site if it was patched
        if (m_pPatchedMostCommonCallAddr) {
            printf("[*] Restoring patched call site...\n");

            uintptr_t callSiteAddr = reinterpret_cast<uintptr_t>(m_pPatchedMostCommonCallAddr);

            // Calculate the original call offset
            uintptr_t originalFuncAddr = reinterpret_cast<uintptr_t>(g_pOriginalSetAng);
            int64_t offset = originalFuncAddr - (callSiteAddr + 5); // 5 = size of E8 call instruction
            int32_t callOffset = static_cast<int32_t>(offset);

            // Restore original 5-byte call instruction
            std::vector<uint8_t> originalInstruction = {
                0xE8,
                static_cast<uint8_t>(callOffset & 0xFF),
                static_cast<uint8_t>((callOffset >> 8) & 0xFF),
                static_cast<uint8_t>((callOffset >> 16) & 0xFF),
                static_cast<uint8_t>((callOffset >> 24) & 0xFF)
            };

            DWORD oldProtect;
            if (!VirtualProtectEx(proc.m_hProc, reinterpret_cast<void*>(callSiteAddr),
                9, PAGE_EXECUTE_READWRITE, &oldProtect)) {
                printf("[!] VirtualProtectEx failed during unhook: %d\n", GetLastError());
                success = false;
            }
            else {
                // Write original call instruction
                if (!proc.WriteArray(callSiteAddr, originalInstruction)) {
                    printf("[!] Failed to restore original call instruction\n");
                    success = false;
                }

                // Restore the next instruction bytes (test sil, 2) that were NOPed
                std::vector<uint8_t> nextInstruction = { 0x40, 0xF6, 0xC6, 0x02 }; // test sil, 2
                if (!proc.WriteArray(callSiteAddr + 5, nextInstruction)) {
                    printf("[!] Failed to restore next instruction\n");
                    success = false;
                }
                else {
                    printf("[+] Restored original call site\n");
                }

                VirtualProtectEx(proc.m_hProc, reinterpret_cast<void*>(callSiteAddr),
                    9, oldProtect, &oldProtect);
            }

            if (!FlushInstructionCache(proc.m_hProc, reinterpret_cast<void*>(callSiteAddr), 9)) {
                printf("[!] Warning: FlushInstructionCache failed: %d\n", GetLastError());
            }
        }

        // Free allocated memory
        if (m_pShellcodeRemote) {
            if (VirtualFreeEx(proc.m_hProc, m_pShellcodeRemote, 0, MEM_RELEASE)) {
                printf("[+] Freed shellcode memory\n");
            }
            else {
                printf("[!] Failed to free shellcode memory: %d\n", GetLastError());
                success = false;
            }
            m_pShellcodeRemote = nullptr;
        }

        if (m_pDataRemote) {
            if (VirtualFreeEx(proc.m_hProc, m_pDataRemote, 0, MEM_RELEASE)) {
                printf("[+] Freed hook data memory\n");
            }
            else {
                printf("[!] Failed to free hook data memory: %d\n", GetLastError());
                success = false;
            }
            m_pDataRemote = nullptr;
        }

        // Reset state
        m_bIsHooked = false;
        m_pTargetFunction = 0;
        m_pPatchedMostCommonCallAddr = nullptr;
        g_pOriginalSetAng = nullptr;
        g_pSetAngHookData = nullptr;

        if (success) {
            printf("[+] CMsgQAngleCpy unhook completed successfully!\n\n");
        }
        else {
            printf("[!] CMsgQAngleCpy unhook completed with errors\n\n");
        }

        return success;
    }

    void CMsgQAngleCpy::ToggleAngleChange() {
        if (!m_pDataRemote)
            return;

        auto bCurrent = proc.ReadDirect<bool>(reinterpret_cast<uintptr_t>(m_pDataRemote) +
            offsetof(CMsgQAngleCpyHookData, bForceQAngle));

        proc.Write<bool>(reinterpret_cast<uintptr_t>(m_pDataRemote) +
            offsetof(CMsgQAngleCpyHookData, bForceQAngle), !bCurrent);
    }

    void CMsgQAngleCpy::SetAngleChange(bool bEnabled) {
        if (!m_pDataRemote)
            return;

        proc.Write<bool>(reinterpret_cast<uintptr_t>(m_pDataRemote) +
            offsetof(CMsgQAngleCpyHookData, bForceQAngle), bEnabled);
    }

    void CMsgQAngleCpy::SetAngle(Vector vAngle) {
        if (!m_pDataRemote)
            return;

        proc.Write<Vector>(reinterpret_cast<uintptr_t>(m_pDataRemote) +
            offsetof(CMsgQAngleCpyHookData, qAngleToSet), vAngle);

    }
}