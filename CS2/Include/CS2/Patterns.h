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