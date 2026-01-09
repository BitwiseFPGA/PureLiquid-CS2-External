# Pure Liquid CS2 [Watch Preview on YouTube](https://www.youtube.com/watch?v=S9A8UnHnU7Y)

## *Advanced External Base For Counter Strike 2 Including TraceShape visibility check, CreateMove Hook for autofire, Silentaim and Chams*  
---

## Features

✨ **Authentic Visibility Checking** - Uses CS2's own TraceShape function for accurate line-of-sight detection  
✨ **External Interface Usage** - Feels like an internal!  
🎯 **External Operation** - No DLL injection required, operates entirely from external process  
🔧 **Real-time Entity Tracking** - Background thread continuously monitors all player entities  
📊 **Dynamic Pattern Scanning** - Automatic function discovery via IDA-style signatures  
🎯 **External CGameEntitySystem** - Resolve Entities easily  
🎯 **Generated SDK** - Fully generated SDK    
🎯 **External CSchemaSystem** - Schema Offset manager      
🎯 **External Aimbot** - with TraceShape Visibility Check     
🎯 **External Silentaim** - Integrated within the aimbot (currently re-hooking doesnt work properly. so if you restart the base you need to restart CS2 )     
🎯 **External CreateMove Hook** - for AImbot Autoshoot and in the feature silent aim  
🎯 **External CAnimatableSceneObjectDesc Hook** - for External Chams  
🎯 **External CUIEngineSource2** - Run script in Context of a panel or just standalone  
🎯 **External Model Change** - Change models of entities externally  
🎯 **External Model Manager** - Collects Hitbox and bone data for given CModel_Imp    

---

---
![CS2 Chams Demo](https://raw.githubusercontent.com/xsip/PureLiquid-CS2-External/refs/heads/main/chams-preview.png)
---

---
![CS2 Chams Demo](https://img.youtube.com/vi/S9A8UnHnU7Y/maxres1.jpg)
---

## Overview

PureLiquid CS2 is a reverse engineering project that implements accurate player visibility detection for Counter-Strike 2 without DLL injection.   
There is also support for Interfaces, PatternScanning, module cloning and so on  
The project demonstrates:  

- **Remote Function Execution** - Execute CS2's internal TraceShape function from an external process
- **External Process Manipulation** - Comprehensive memory operations without traditional injection techniques

---


## How It Works

The system performs visibility checks by:

1. **Pattern Scanning** - Locates CS2's internal TraceShape function using byte signatures
2. **Context Preparation** - Builds reusable TraceFilter and Ray structures in remote memory
3. **Shellcode Injection** - Copies lightweight wrapper code to CS2's address space
4. **Remote Execution** - Creates thread in CS2 that calls the real TraceShape function
5. **Result Retrieval** - Reads trace results to determine if line-of-sight exists

This approach provides the same accuracy as CS2's internal checks while operating entirely externally.

---

## Usage Example Visibility Check

```cpp
int main() {
    
    SetConsoleTitle("xsip's external traceshape visibility check");
    
    // Initialize CS2 interfaces
    I::Initialize();
    
    // Start entity tracking thread
    std::thread([]() { ReadEntititesThread(); }).detach();

    auto pGameEntitySystem = I::pGameResourceService->GetGameEntitySystem();

    // Get local player
    auto lpController = pGameEntitySystem->GetEntityByIndex<CS2::client::CCSPlayerController>(1);
    auto lpPawn = pGameEntitySystem->GetEntityByIndex<CS2::client::C_CSPlayerPawn>(
        lpController->m_hPawn.GetEntryIndex()
    );
    
    // Main loop: Check visibility when left shift is pressed
    while (true) {
        if (!GetAsyncKeyState(VK_LSHIFT)) {
            Sleep(500);
            continue;
        }

        system("cls");
        for (int i = 1; i < 65; i++) {
            auto entity = &CGameEntitySystem::vEntityList[i];

            if (!entity->m_bIsValid || !entity->m_bIsAlive || entity->m_bIsLocalPlayer)
                continue;

            // Perform visibility check using CS2's internal TraceShape
            bool visible = I::pGameTraceManager->IsPlayerVisible(lpPawn, entity->m_pPawn);
            
            printf("Entity %i: %s\n", i, visible ? "Visible" : "Invisible");
        }
    }

    return 0;
}
```

**Example Output:**
```
Entity 2: Visible
Entity 3: Invisible
Entity 5: Visible
Entity 7: Invisible
Entity 9: Visible
```

---

## External CCSGOInput Usage:

```c++

int main() {
	
	I::Initialize();
	while (true) {
		if (GetAsyncKeyState(VK_LSHIFT)) {
        // Getting angles
			auto v = I::pCsGoInput->vViewAngles;
			printf("View: %.2f %.2f %.2f\n", v.x, v.y, v.z);
        // Setting angles:
        I::pCsGoInput->vViewAngles = {0.0f, 0.0f, 0.0f};
		}
	}
}

```
---

## External Model Changer Usage:

```c++

int main() {
	I::Initialize();
	auto pGameEntitySystem = I::pGameResourceService->GetGameEntitySystem();

	auto lpController = pGameEntitySystem->GetEntityByIndex<CS2::client::CCSPlayerController>(1);
	auto lpPawn = pGameEntitySystem->GetEntityByIndex<CS2::client::C_CSPlayerPawnExtended>(lpController->m_hPawn.GetEntryIndex());
	while (true) {
		if (GetAsyncKeyState(VK_LSHIFT)) {
			CModelChanger::ChangeModel(lpPawn, "models/inventory_items/dogtags.vmdl"); // you need to be sure that the model is loaded.
			// Loading models will be implemented in the future..
			printf("Model Changed!\n");
			Sleep(1000);
		}
	}
	return 1;

```

---


---

## External CSchemaSystem Offset Manager Usage:  

```c++
int main() {
	
	SetConsoleTitle("PureLiquid CS2 External");
	
	I::Initialize();


	while (true) {
		if (GetAsyncKeyState(VK_LSHIFT)) {
			printf("m_hPawn: 0x%x\n", I::pSchemaSystem->GetOffset("client.dll->CBasePlayerController->m_hPawn")); // Get offset by key
			printf("m_hPawn: 0x%x\n", I::pSchemaSystem->GetOffset("client.dll", "CBasePlayerController", "m_hPawn")); // Get offset by module, class and field name
			continue;
		}
		Sleep(1000);
	}

	return 1;

}
```

## External Model Manager Usage:

```c++

int main() {

	I::Initialize();
	auto pGes = I::pGameResourceService->GetGameEntitySystem();

	auto lpC = pGes->GetEntityByIndex<CS2::client::CCSPlayerController>(1);
	auto lpP = pGes->GetEntityByIndex<CS2::client::C_CSPlayerPawnExtended>(lpC->m_hPawn.GetEntryIndex());

	while (true) {

		if (GetAsyncKeyState(VK_LSHIFT)) {
			auto pModel = lpP->GetCModel_Imp();
			auto pHeadData = pModelManager->GetHitboxAndBoneData(pModel, 6);

			printf("Bone: %s\n", pHeadData.hitbox.m_szBoneName.c_str());
			printf("\tIndex: %i\n", pHeadData.hitbox.m_iBoneIdx);
			printf("Hitbox: %s\n", pHeadData.hitbox.m_szName.c_str());
			printf("\tIndex: %i\n", pHeadData.hitbox.m_nHitBoxIndex);

			printf("\tHitbox vMin: %.2f %.2f %.2f\n", pHeadData.hitbox.m_vMinBounds.x, pHeadData.hitbox.m_vMinBounds.y, pHeadData.hitbox.m_vMinBounds.z);
			printf("\tHitbox vMax: %.2f %.2f %.2f\n", pHeadData.hitbox.m_vMaxBounds.x, pHeadData.hitbox.m_vMaxBounds.y, pHeadData.hitbox.m_vMaxBounds.z);
			printf("\tHitbox FlShapeRadius: %.2f\n", pHeadData.hitbox.m_flShapreRadius);
		}

	}

}
```


---

## External CreateMove hook Usage:

```c++

int main() {
	
	I::Initialize();
	I::pCsGoInput->HookCreateMove();
	while (!GetAsyncKeyState(VK_DELETE)) {
		if (GetAsyncKeyState(VK_LSHIFT)) {
			I::pCsGoInput->Attack(); // Writes Attack in CreateMove
   			I::pCsGoInput->SetSubTickAngle({ aimAngle.x,aimAngle.y,aimAngle.z}); // writes angle in createmove cmd
		}
	}
	I::pCsGoInput->UnhookCreateMove(); // Unhook on shutdown
}

```

---

## External Chams Usage:

```c++

int main() {
	
	I::Initialize();
	// have a look at GetLatexChams and create your own if you wish to
	auto hLatexChamsMaterial = I::pMaterialSystem->CreateMaterial(CMaterialSystem2::GetLatexChams(), "LatexChamsMaterial");

	if (!hLatexChamsMaterial) {
		printf("Couldn't create Material!!\n");
		return 0;
	}

	if (!CAnimatableSceneObjectDesc::InstallRendererHook(nullptr)) { // nullptr could be replaced by hLatexChamsMaterial to set it directly
		printf("Error Installing CAnimatableSceneObjectDesc hook!!\n");
		return 0;
	}


	CAnimatableSceneObjectDesc::SetChamsColor(0, 35, 255, 255);
	CAnimatableSceneObjectDesc::SetChamsMaterial(hLatexChamsMaterial);
	CAnimatableSceneObjectDesc::SetChamsEnabled(true);

	while (!GetAsyncKeyState(VK_DELETE)) {
		
		// CAnimatableSceneObjectDesc::SetChamsColor(0, 35, 255, 255); Change color
		// CAnimatableSceneObjectDesc::SetChamsMaterial(hLatexChamsMaterial); // Change Material ( you need to call CreateMaterial first )
		// CAnimatableSceneObjectDesc::SetChamsEnabled(true); // Enable/ Disable Materials
	}
	CAnimatableSceneObjectDesc::UninstallRendererHook(); // Uninstall hook at exit!
}

```

## External RunScript Usage:

```c++

int main() {
	
	I::Initialize();
	while (!GetAsyncKeyState(VK_DELETE)) {
		if (GetAsyncKeyState(VK_LSHIFT)) {
			I::pPanoramaUIEngine->GetCUIEngineSource2()->RunScript("$.Msg(\"Hello, world123!\");"); // Will output "Hello, World123" in the console.
		}
	}
}

```


---

## Building

**Requirements:**
- Visual Studio
- Windows 10/11 SDK
- MASM (Microsoft Macro Assembler)

**Steps:**
1. Open `PureLiquid-CS2-External.slnx`
2. Set configuration to **Release | x64**
3. Build solution (Ctrl+Shift+B)
4. Output: `Bin/x64/Release/External.exe`

---

## Information

⚠️ **Educational Purpose Only** - This project is for learning about:
- Windows process internals and memory manipulation
- Game engine reverse engineering
- Direct syscall invocation

Compile in **Release mode** for optimal performance and correct shellcode generation.

---

## Credits

- **Unknowncheats** for CS2 structure research and entity system documentation

---

## License

This project is provided for **educational and research purposes only**. Using this software in online games violates Terms of Service and may be illegal. The author assumes no responsibility for misuse.

---

## Other Projects
[CS2-External-Chams](https://github.com/xsip/CS2-External-Chams)
