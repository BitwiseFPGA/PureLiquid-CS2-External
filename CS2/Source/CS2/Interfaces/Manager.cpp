#include <CS2/Interfaces/Manager.h>
#include <CS2/Interfaces/Include.h>
namespace CS2 {
	void CInterfaceManager::LogAll() {
		printf("[+] Interfaces\n");
		printf("[+] CGameResourceService: 0x%p\n", pGameResourceService);
		printf("[+] CGameResourceService->GetGameEntitySystem(): 0x%p\n", pGameResourceService->GetGameEntitySystem());
		printf("[+] CPanoramaUIEngine: 0x%p\n", pPanoramaUIEngine);
		printf("[+] CGameTraceManager: 0x%p\n", pGameTraceManager);
		printf("[+] CMaterialSystem2: 0x%p\n", pMaterialSystem);
		printf("[+] CSource2Client: 0x%p\n", pSource2Client);
		printf("[+] CSource2Client->GetSceneDebugOverlay(): 0x%p\n", pSource2Client->GetSceneDebugOverlay());
		printf("[+] CLegacyGameUI: 0x%p\n", pLegacyGameUI);
		printf("[+] CCSGOInput: 0x%p\n", pCsGoInput);
		printf("[+] CCSGO_Hud: 0x%p\n", pCsGoHud);
		printf("[+] CSchemaSystem: 0x%p\n\n", pSchemaSystem);
	}
}