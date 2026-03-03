#pragma once


// client.dll => @xref "Attempting to get hit box list and transforms for an uninitialized model"
/*
text:0000000000763044 48 8D 15 35 8F EF 00                    lea     rdx, aAttemptingToGe ; "Attempting to get hit box list and tran"...
.text:000000000076304B 48 8D 0D D6 8E EF 00                    lea     rcx, off_165BF28 ; "C:\\buildworker\\csgo_rel_win64\\build"...
.text:0000000000763052 89 05 B4 3C 6C 01                       mov     cs:dword_1E26D0C, eax
.text:0000000000763058 FF 15 1A D9 D7 00                       call    cs:?Assert_ConditionFailed@@YA_NAEBU_AssertCompileTimeConstantStruct_t@@PEBDZZ ; Assert_ConditionFailed(_AssertCompileTimeConstantStruct_t const &,char const *,...)
.text:000000000076305E 84 C0                                   test    al, al
.text:0000000000763060 74 01                                   jz      short loc_763063
.text:0000000000763062 CC                                      int     3               ; Trap to Debugger
.text:0000000000763063
.text:0000000000763063                         loc_763063:                             ; CODE XREF: sub_762FF0+21↑j
.text:0000000000763063                                                                 ; sub_762FF0+50↑j ...
.text:0000000000763063 32 C0                                   xor     al, al
.text:0000000000763065
.text:0000000000763065                         loc_763065:                             ; CODE XREF: sub_762FF0+B7↓j
.text:0000000000763065                                                                 ; sub_762FF0+238↓j
.text:0000000000763065 48 8B 9C 24 C0 00 00 00                 mov     rbx, [rsp+0A8h+arg_10]
.text:000000000076306D 48 81 C4 80 00 00 00                    add     rsp, 80h
.text:0000000000763074 41 5F                                   pop     r15
.text:0000000000763076 41 5D                                   pop     r13
.text:0000000000763078 5F                                      pop     rdi
.text:0000000000763079 5E                                      pop     rsi
.text:000000000076307A 5D                                      pop     rbp
.text:000000000076307B C3                                      retn
.text:000000000076307C                         ; ---------------------------------------------------------------------------
.text:000000000076307C
.text:000000000076307C                         loc_76307C:                             ; CODE XREF: sub_762FF0+45↑j
.text:000000000076307C BA 00 01 00 00                          mov     edx, 100h
.text:0000000000763081
.text:0000000000763081                         loc_763081:                             ; DATA XREF: .rdata:00000000019ECB84↓o
.text:0000000000763081                                                                 ; .rdata:00000000019ECB94↓o ...
.text:0000000000763081 4C 89 A4 24 B0 00 00 00                 mov     [rsp+0A8h+arg_0], r12
.text:0000000000763089 48 8B CB                                mov     rcx, rbx
.text:000000000076308C E8 DF 08 FF FF                          call    sub_753970
.text:0000000000763091 4C 8B A3 10 02 00 00                    mov     r12, [rbx+210h] ; bone matrix Pointer
*/
#define BONES_PTR_OFFSET_PATTERN "4C 8B A3 ?? ?? ?? ?? 4D 85 E4"



// @client.dll - i also posted about this on UC: https://www.unknowncheats.me/forum/counter-strike-2-a/733064-easy-silent-aim.html
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
#define SET_SERVER_ANGLE_CALLSITE_PATTERN "E8 ?? ?? ?? ?? 40 F6 C6 ?? 74 ?? 83 0F ?? 48 8B 43"

// client.dll => CClientInput @ Index 7
// text:0000000000821DB4 89 86 B8 06 00 00                       mov     [rsi+6B8h], eax
#define C_CSGO_INPUT_VIEW_ANGLES_PATTERN "F2 0F 11 86 ?? ?? ?? ?? 89 86 ?? ?? ?? ?? C7 86"


// @xref => models/inventory_items/dogtags.vmdl @ client.dll
#define CHANGE_MODEL_PATTERN "40 53 48 83 EC ?? 48 8B D9 4C 8B C2 48 8B 0D ?? ?? ?? ?? 48 8D 54 24"


// @xref => Physics/TraceShape - client.dll
#define TRACESHAPE_PATTERN "48 89 5C 24 ?? 48 89 4C 24 ?? 55 57"




// client @xref =>  [%d] Could not find Hud Element: %s\n
#define CCSGO_HUD_PATTERN "48 8B 05 ?? ?? ?? ?? 48 8B D9 48 85 C0 0F 84 ?? ?? ?? ?? 49 89 5B"

// client.dll =>
// .text:0000000000200C59 C6 44 24 20 0F								mov[rsp + 38h + var_18], 0Fh
// .text : 0000000000200C5E E8 3D 0C 00 00								call    TraceInitEntitiesOnly_sub_2018A0
// .text : 0000000000200C63 48 81 4B 10 00 00 0C 00						or qword ptr[rbx + 10h], 0C0000h
// .text : 0000000000200C6B 48 8D 05 7E 3A 33 01						lea     rax, ? ? _7CAnimGraphTraceFilter@@6B@; const CAnimGraphTraceFilter::`vftable'
#define INIT_TRACE_ENTITIES_ONLY_PATTERN "48 89 5C 24 ?? 48 89 74 24 ?? 57 48 83 EC ?? 0F B6 41 ?? 33 FF 24"



// Interfaces @ client.dll
#define C_CSGO_INPUT_PATTERN "48 8B 0D ?? ?? ?? ?? 4C 8D 47 14"
#define C_GAME_TRACE_MANAGER_PATTERN "4C 8B 2D ? ? ? ? 24"



// materialsystem.dll =>
// rdata:000000000012A188; class CMaterialSystem2 : CTier2AppSystem<IMaterialSystem2, 0>, CTier1AppSystem<IMaterialSystem2, 0>, CTier0AppSystem<IMaterialSystem2, 0>, CBaseAppSystem<IMaterialSystem2>, IMaterialSystem2, IAppSystem;  (#classinformer)
// .rdata:000000000012A188                 dq offset ? ? _R4CMaterialSystem2@@6B@; const CMaterialSystem2::`RTTI Complete Object Locator'
// .rdata:000000000012A190; const CMaterialSystem2::`vftable'
// .rdata:000000000012A190 ? ? _7CMaterialSystem2@@6B@ dq offset sub_38140
// .rdata:000000000012A190; DATA XREF : sub_36090 + 15↑o
// .rdata : 000000000012A190; sub_364D0 + B↑o
// .rdata:000000000012A198                 dq offset sub_381B0
// .rdata : 000000000012A1A0                 dq offset antlr3dfaspecialTransition
// .rdata : 000000000012A1A8                 dq offset sub_36A80
// .rdata : 000000000012A1B0                 dq offset sub_37F20
// .rdata : 000000000012A1B8                 dq offset sub_37F10
// .rdata : 000000000012A1C0                 dq offset sub_36080
// .rdata : 000000000012A1C8                 dq offset sub_44EE0
// .rdata : 000000000012A1D0                 dq offset sub_44EF0
// .rdata : 000000000012A1D8                 dq offset sub_34810
// .rdata : 000000000012A1E0                 dq offset sub_44EE0
// .rdata : 000000000012A1E8                 dq offset sub_381F0
// .rdata : 000000000012A1F0                 dq offset sub_38450
// .rdata : 000000000012A1F8                 dq offset sub_38460
// .rdata : 000000000012A200                 dq offset sub_38470
// .rdata : 000000000012A208                 dq offset sub_38540
// .rdata : 000000000012A210                 dq offset sub_38520
// .rdata : 000000000012A218                 dq offset sub_386B0
// .rdata : 000000000012A220                 dq offset sub_38760
// .rdata : 000000000012A228                 dq offset sub_38780
// .rdata : 000000000012A230                 dq offset sub_3B600
// .rdata : 000000000012A238                 dq offset sub_392B0
// .rdata : 000000000012A240                 dq offset sub_38120
// .rdata : 000000000012A248                 dq offset sub_3C9D0
// .rdata : 000000000012A250                 dq offset sub_3CC00
// .rdata : 000000000012A258                 dq offset sub_3CD30
// .rdata : 000000000012A260                 dq offset sub_3B9A0
// .rdata : 000000000012A268                 dq offset sub_3B9B0
// .rdata : 000000000012A270                 dq offset sub_3BFA0
// .rdata : 000000000012A278                 dq offset CreateMaterial_sub_3BE30 <= this
// .rdata : 000000000012A280                 dq offset sub_3BC30
/*

text:000000000003BE30                 mov     [rsp+arg_0], rbx
.text:000000000003BE35                 mov     [rsp+arg_10], rbp
.text:000000000003BE3A                 push    rsi
.text:000000000003BE3B                 push    rdi
.text:000000000003BE3C                 push    r14
.text:000000000003BE3E                 sub     rsp, 110h
.text:000000000003BE45                 mov     rax, cs:g_pMemAlloc
.text:000000000003BE4C                 mov     rsi, rdx
.text:000000000003BE4F                 mov     edx, 690h
.text:000000000003BE54                 mov     r14, r9
.text:000000000003BE57                 mov     rbp, r8
.text:000000000003BE5A                 mov     rcx, [rax]
.text:000000000003BE5D                 mov     rax, [rcx]
.text:000000000003BE60                 call    qword ptr [rax+8]
.text:000000000003BE63                 xor     edi, edi
.text:000000000003BE65                 mov     rbx, rax
.text:000000000003BE68                 test    rax, rax
.text:000000000003BE6B                 jz      short loc_3BEE1
.text:000000000003BE6D                 and     byte ptr [rbx+0Bh], 0C0h
.text:000000000003BE71                 lea     rax, ??_7CMaterial2@@6B@ ; const CMaterial2::`vftable' <= First function containing CMaterial2 VTable
.text:000000000003BE78                 mov     [rbx], rax
.text:000000000003BE7B                 mov     eax, 1
.text:000000000003BE80                 mov     dword ptr [rbx+0Ch], 0FFFFFFFFh
*/
#define CREATE_MATERIAL_FN_PATTERN "48 89 5C 24 ?? 48 89 6C 24 ?? 56 57 41 56 48 81 EC ?? ?? ?? ?? 48 8B 05"


// tier0.dll => exported function.
#define LOAD_KV3_PROC_ADDRESS "?LoadKV3@@YA_NPEAVKeyValues3@@PEAVCUtlString@@PEBDAEBUKV3ID_t@@2I@Z"

// client.dll
#define GET_VIEW_MATRIX_FN "48 63 C1 48 8D 0D ?? ?? ?? ?? 48 C1 E0 06"

// scenesystem.dll =>
// .rdata:0000000000593D28                         ; class CAnimatableSceneObjectDesc: CBaseSceneObjectDesc, ISceneObjectDesc;  (#classinformer)
// .rdata:0000000000593D28 D0 34 5B 00 00 00 00 00                 dq offset ??_R4CAnimatableSceneObjectDesc@@6B@ ; const CAnimatableSceneObjectDesc::`RTTI Complete Object Locator'
// .rdata:0000000000593D30                         ; const VCAnimatableSceneObjectDesc::`vftable'
// .rdata:0000000000593D30 20 C7 04 00 00 00 00 00 ??_7VCAnimatableSceneObjectDesc@@6B@ dq offset sub_4C720
// .rdata:0000000000593D30                                                                 ; DATA XREF: .data:off_61B2F0↓o
// .rdata:0000000000593D38 A0 DF 04 00 00 00 00 00                 dq offset RenderAnimatable_Objects_sub_4DFA0 <= This function

#define CANIMATABLE_SCENE_OBJECT_DESC_RENDER_FN_PATTERN "48 8B C4 53 57 41 54 48 81 EC ?? ?? ?? ?? 49 63 F9 49"

// @ panorama.dll => xref CUIEngine::RunScript (compile+run)
#define RUN_SCRIPT_PATTERN "48 89 5C 24 18 4C 89 4C 24 20 48 89 54 24 10 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 80"




// @client.dll
/*

text:0000000000BF1DB7 48 8D 15 FA 4E EE 00                    lea     rdx, aSfuiNoticeDmBu_2 ; "#SFUI_Notice_DM_BuyMenu_RandomOFF"
.text:0000000000BF1DBE
.text:0000000000BF1DBE                         loc_BF1DBE:                             ; CODE XREF: sub_BF1850+446↑j
.text:0000000000BF1DBE                                                                 ; sub_BF1850+45C↑j
.text:0000000000BF1DBE 41 FF 50 78                             call    qword ptr [r8+78h]
.text:0000000000BF1DC2 4C 89 74 24 38                          mov     qword ptr [rsp+580h+var_548], r14
.text:0000000000BF1DC7 48 8D 8D A0 04 00 00                    lea     rcx, [rbp+480h+arg_10]
.text:0000000000BF1DCE 48 89 4C 24 30                          mov     [rsp+580h+var_550], rcx
.text:0000000000BF1DD3 48 8D 55 20                             lea     rdx, [rbp+480h+var_460]
.text:0000000000BF1DD7 48 8B 0D EA 78 92 01                    mov     rcx, cs:qword_25196C8
.text:0000000000BF1DDE 4C 8B C8                                mov     r9, rax
.text:0000000000BF1DE1 48 89 5C 24 28                          mov     [rsp+580h+var_558], rbx
.text:0000000000BF1DE6 41 B8 00 04 00 00                       mov     r8d, 400h
.text:0000000000BF1DEC C7 44 24 20 03 00 00 00                 mov     dword ptr [rsp+580h+var_560], 3
.text:0000000000BF1DF4 41 FF D7                                call    r15
.text:0000000000BF1DF7
.text:0000000000BF1DF7                         loc_BF1DF7:                             ; CODE XREF: sub_BF1850+520↑j
.text:0000000000BF1DF7 BA 01 80 00 00                          mov     edx, 8001h
.text:0000000000BF1DFC 48 8D 4D 20                             lea     rcx, [rbp+480h+var_460]
.text:0000000000BF1E00 E8 AB 35 1F 00                          call    sub_DE53B0
.text:0000000000BF1E05 F3 0F 10 87 70 3D 00 00                 movss   xmm0, dword ptr [rdi+3D70h]
.text:0000000000BF1E0D 48 8D 4C 24 50                          lea     rcx, [rsp+580h+var_530]
.text:0000000000BF1E12 33 D2                                   xor     edx, edx
.text:0000000000BF1E14 F3 0F 11 87 78 3D 00 00                 movss   dword ptr [rdi+3D78h], xmm0
.text:0000000000BF1E1C FF 15 F6 FA D0 00                       call    cs:?Purge@CBufferString@@QEAAXH@Z ; CBufferString::Purge(int)
.text:0000000000BF1E22 33 D2                                   xor     edx, edx
.text:0000000000BF1E24 48 8D 4D B0                             lea     rcx, [rbp+480h+var_4D0]
.text:0000000000BF1E28 FF 15 EA FA D0 00                       call    cs:?Purge@CBufferString@@QEAAXH@Z ; CBufferString::Purge(int)
.text:0000000000BF1E2E 4C 8B BC 24 50 05 00 00                 mov     r15, [rsp+580h+var_30]
.text:0000000000BF1E36
.text:0000000000BF1E36                         loc_BF1E36:                             ; CODE XREF: sub_BF1850+25A↑j
.text:0000000000BF1E36                                                                 ; DATA XREF: .pdata:00000000025B4A70↓o ...
.text:0000000000BF1E36 48 8D 4D F0                             lea     rcx, [rbp+480h+var_490]
.text:0000000000BF1E3A E8 E1 3F EE FF                          call    sub_AD5E20
.text:0000000000BF1E3F
.text:0000000000BF1E3F                         loc_BF1E3F:                             ; CODE XREF: sub_BF1850+1B0↑j
.text:0000000000BF1E3F                                                                 ; sub_BF1850+1BD↑j ...
.text:0000000000BF1E3F 40 38 B7 6C 3D 00 00                    cmp     [rdi+3D6Ch], sil
.text:0000000000BF1E46 74 6E                                   jz      short loc_BF1EB6
.text:0000000000BF1E48 40 38 B7 A9 15 00 00                    cmp     [rdi+15A9h], sil
.text:0000000000BF1E4F 74 6E                                   jz      short loc_BF1EBF
.text:0000000000BF1E51 48 8B 07                                mov     rax, [rdi]
.text:0000000000BF1E54 48 8B CF                                mov     rcx, rdi
.text:0000000000BF1E57 FF 90 B0 04 00 00                       call    qword ptr [rax+4B0h]
.text:0000000000BF1E5D 84 C0                                   test    al, al
.text:0000000000BF1E5F 74 55                                   jz      short loc_BF1EB6
.text:0000000000BF1E61 0F B6 87 F3 03 00 00                    movzx   eax, byte ptr [rdi+3F3h]
.text:0000000000BF1E68 2C 02                                   sub     al, 2
.text:0000000000BF1E6A 41 3A C4                                cmp     al, r12b
.text:0000000000BF1E6D 77 47                                   ja      short loc_BF1EB6
.text:0000000000BF1E6F BA FF FF FF FF                          mov     edx, 0FFFFFFFFh
.text:0000000000BF1E74 48 8D 0D 5D D1 71 01                    lea     rcx, unk_230EFD8
.text:0000000000BF1E7B E8 F0 A1 C1 00                          call    sub_180C070
.text:0000000000BF1E80 48 85 C0                                test    rax, rax
.text:0000000000BF1E83 75 0B                                   jnz     short loc_BF1E90
.text:0000000000BF1E85 48 8B 05 54 D1 71 01                    mov     rax, cs:qword_230EFE0
.text:0000000000BF1E8C 48 8B 40 08                             mov     rax, [rax+8]
.text:0000000000BF1E90
.text:0000000000BF1E90                         loc_BF1E90:                             ; CODE XREF: sub_BF1850+633↑j
.text:0000000000BF1E90 48 8D 15 49 4E EE 00                    lea     rdx, aBuyrandom ; "buyrandom"
.text:0000000000BF1E97 48 8B 0D 3A 2D 71 01                    mov     rcx, cs:qword_2304BD8
.text:0000000000BF1E9E 40 38 30                                cmp     [rax], sil
.text:0000000000BF1EA1 75 07                                   jnz     short loc_BF1EAA
.text:0000000000BF1EA3 48 8D 15 E2 95 ED 00                    lea     rdx, aRebuy     ; "rebuy"
.text:0000000000BF1EAA
.text:0000000000BF1EAA                         loc_BF1EAA:                             ; CODE XREF: sub_BF1850+651↑j
.text:0000000000BF1EAA E8 B1 1A F5 FF                          call    sub_B43960
.text:0000000000BF1EAF 40 88 B7 6C 3D 00 00                    mov     [rdi+3D6Ch], sil
.text:0000000000BF1EB6
.text:0000000000BF1EB6                         loc_BF1EB6:                             ; CODE XREF: sub_BF1850+5F6↑j
.text:0000000000BF1EB6                                                                 ; sub_BF1850+60F↑j ...
.text:0000000000BF1EB6 40 38 B7 A9 15 00 00                    cmp     [rdi+15A9h], sil
.text:0000000000BF1EBD 75 08                                   jnz     short loc_BF1EC7
.text:0000000000BF1EBF
.text:0000000000BF1EBF                         loc_BF1EBF:                             ; CODE XREF: sub_BF1850+5FF↑j
.text:0000000000BF1EBF 48 8B CF                                mov     rcx, rdi
.text:0000000000BF1EC2 E8 89 E9 02 00                          call    CallsGetSpread_sub_C20850 <= We hook inside this function



.text:0000000000C20850                         CallsGetSpread_sub_C20850 proc near     ; CODE XREF: sub_BF1850+672↑p
.text:0000000000C20850                                                                 ; sub_C14BA0+139↑p
.text:0000000000C20850                                                                 ; DATA XREF: ...
.text:0000000000C20850
.text:0000000000C20850                         var_38          = xmmword ptr -38h
.text:0000000000C20850                         var_28          = xmmword ptr -28h
.text:0000000000C20850                         var_18          = xmmword ptr -18h
.text:0000000000C20850                         arg_0           = qword ptr  8
.text:0000000000C20850                         arg_8           = qword ptr  10h
.text:0000000000C20850
.text:0000000000C20850 48 89 5C 24 10                          mov     [rsp+arg_8], rbx
.text:0000000000C20855 57                                      push    rdi
.text:0000000000C20856 48 83 EC 50                             sub     rsp, 50h
.text:0000000000C2085A 48 8B D9                                mov     rbx, rcx
.text:0000000000C2085D E8 8E 35 C2 FF                          call    sub_843DF0
.text:0000000000C20862 48 8B F8                                mov     rdi, rax
.text:0000000000C20865 48 85 C0                                test    rax, rax
.text:0000000000C20868 0F 84 C0 00 00 00                       jz      loc_C2092E
.text:0000000000C2086E
.text:0000000000C2086E                         loc_C2086E:                             ; DATA XREF: .rdata:0000000001E75878↓o
.text:0000000000C2086E                                                                 ; .rdata:0000000001E75894↓o ...
.text:0000000000C2086E 48 89 74 24 60                          mov     [rsp+58h+arg_0], rsi
.text:0000000000C20873 48 8B B3 10 14 00 00                    mov     rsi, [rbx+1410h]
.text:0000000000C2087A 48 85 F6                                test    rsi, rsi
.text:0000000000C2087D 0F 84 A6 00 00 00                       jz      loc_C20929
.text:0000000000C20883 48 8B 08                                mov     rcx, [rax]
.text:0000000000C20886 45 33 C0                                xor     r8d, r8d
.text:0000000000C20889
.text:0000000000C20889                         loc_C20889:                             ; DATA XREF: .rdata:0000000001E75894↓o
.text:0000000000C20889                                                                 ; .rdata:0000000001E758A4↓o ...
.text:0000000000C20889 0F 29 74 24 40                          movaps  [rsp+58h+var_18], xmm6
.text:0000000000C2088E 33 D2                                   xor     edx, edx
.text:0000000000C20890 0F 29 7C 24 30                          movaps  [rsp+58h+var_28], xmm7
.text:0000000000C20895 44 0F 29 44 24 20                       movaps  [rsp+58h+var_38], xmm8
.text:0000000000C2089B 4C 8B 89 D0 0C 00 00                    mov     r9, [rcx+0CD0h]
.text:0000000000C208A2 48 8B C8                                mov     rcx, rax
.text:0000000000C208A5 41 FF D1                                call    r9
.text:0000000000C208A8 48 8B 07                                mov     rax, [rdi]
.text:0000000000C208AB 48 8B CF                                mov     rcx, rdi
.text:0000000000C208AE 44 0F 28 C0                             movaps  xmm8, xmm0
.text:0000000000C208B2 FF 90 A8 0B 00 00                       call    qword ptr [rax+0BA8h]
.text:0000000000C208B8 48 8B 07                                mov     rax, [rdi]
.text:0000000000C208BB 48 8B CF                                mov     rcx, rdi
.text:0000000000C208BE F3 44 0F 58 C0                          addss   xmm8, xmm0
.text:0000000000C208C3 FF 90 18 0C 00 00                       call    qword ptr [rax+0C18h]
.text:0000000000C208C9 83 F8 02                                cmp     eax, 2
.text:0000000000C208CC 75 0A                                   jnz     short loc_C208D8
.text:0000000000C208CE F3 0F 10 3D AA CC DD 00                 movss   xmm7, cs:dword_19FD580
.text:0000000000C208D6 EB 08                                   jmp     short loc_C208E0
.text:0000000000C208D8                         ; ---------------------------------------------------------------------------
.text:0000000000C208D8
.text:0000000000C208D8                         loc_C208D8:                             ; CODE XREF: CallsGetSpread_sub_C20850+7C↑j
.text:0000000000C208D8 F3 0F 10 3D 08 94 CF 00                 movss   xmm7, cs:dword_1919CE8
.text:0000000000C208E0
.text:0000000000C208E0                         loc_C208E0:                             ; CODE XREF: CallsGetSpread_sub_C20850+86↑j
.text:0000000000C208E0 48 8B 1F                                mov     rbx, [rdi]
.text:0000000000C208E3 45 33 C0                                xor     r8d, r8d
.text:0000000000C208E6 33 D2                                   xor     edx, edx
.text:0000000000C208E8 48 8B CF                                mov     rcx, rdi
.text:0000000000C208EB FF 93 D0 0C 00 00                       call    qword ptr [rbx+0CD0h] ; Calls Get Spread ( We hook this )

*/
#define GET_SPREAD_CALLSITE_PATTERN "FF 93 ?? ?? ?? ?? BA 01 00 00 00 48 8B CF 0F 28 F0"
