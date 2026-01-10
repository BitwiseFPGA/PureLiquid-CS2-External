
#undef USE_CHAMS
#include <External/Include.h>
#include <Source2/CBaseHandle.h>
#include <CS2/SDK/client/CCSPlayerController.hpp>
#include <CS2/SDK/modellib/CRenderMesh.hpp>
#include <CS2/ExtendedSDK/scenesystem/CAnimatableSceneObjectDesc.h>
#include <CS2/ExtendedSDK/client/C_CSPlayerPawn.h>
#include <CS2/ExtendedSDK/client/CGameSceneNode.h>
#include <CS2/ExtendedSDK/resourcesystem/CModel_Imp.h>
#include <CS2/ExtendedSDK/modellib/CRenderMesh.h>
#include <CS2/SDK/modellib/CRenderSkeleton.hpp>
#include <CS2/SDK/modellib/RenderSkeletonBone_t.hpp>
#include <CS2/Interfaces/Include.h>
#include <Features/Aimbot.h>
#include <Features/CModelChanger.h>
#include <CS2/Managers/CModelManager.h>
#include <CS2/Hooks/Client/CMsgQAngleCpyHook.h>
#include <CS2/Panorama/CUI.h>
#define USE_CHAMS
#define USE_CREATE_MOVE
#define USE_SILENT_AIM
// #define USE_CHAMS_VISIBILITY_BASED
namespace Globals {
	Process proc{ "cs2.exe" };
}
// cpy QMsgAngle fn => client @ E8 ?? ?? ?? ?? 40 F6 C5 ?? 74 ?? 8B 46 ?? 89 47 ?? 09 2B
using namespace Globals;
using namespace CS2;


void ReadEntititesThread() {
	int bone = 0;
	auto pGameEntitySystem = CS2::I::pGameResourceService->GetGameEntitySystem();

	while (true) {
#ifdef USE_CHAMS_VISIBILITY_BASED 
		std::array<uint32_t, MAX_VISIBLE_PLAYERS> visibleIndexes{};
		size_t visibleCount = 0;
#endif

		auto pLocalEntity = CGameEntitySystem::GetLocalPlayer();

		for (int i = 1; i < 65; i++) {
			
			auto entity = &CS2::CGameEntitySystem::vEntityList[i];
			auto pController = pGameEntitySystem->GetEntityByIndex<CS2::client::CCSPlayerController>(i);

			if (!pController) {
				entity->m_bIsValid = false;
				continue;
			}

			auto bIsLocalPlayer = pController->m_bIsLocalPlayerController;

			if (bIsLocalPlayer)
				CGameEntitySystem::lpIndex = i;

			if (!pController->m_hPawn.IsValid()) {
				entity->m_bIsValid = false;
				continue;
			}

			auto pPawn = pGameEntitySystem->GetEntityByIndex<CS2::client::C_CSPlayerPawnExtended>(pController->m_hPawn.GetEntryIndex());

			if (!pPawn) {
				entity->m_bIsValid = false;
				continue;
			}

			auto health = pPawn->m_iHealth;
			entity->m_bIsAlive = health > 0 && health <= pPawn->m_iMaxHealth;
			entity->m_iPawnIndex = pController->m_hPawn.GetEntryIndex();
			entity->m_bIsValid = true;
			entity->m_bIsLocalPlayer = bIsLocalPlayer;
			entity->m_pController = pController;
			entity->m_pPawn = pPawn;
			

#ifdef USE_CHAMS_VISIBILITY_BASED 
			

			if (entity->m_bIsLocalPlayer || !pLocalEntity)
				continue;
			
			bool bIsVisible = false;
			if (entity->m_bIsAlive) {
				bIsVisible = I::pGameTraceManager->IsPlayerVisible(lpEntity->m_pPawn, entity->m_pPawn);
			}


			entity->m_bIsVisible = bIsVisible;


			if (entity->m_bIsVisible) {
				visibleIndexes[visibleCount++] = entity->m_iPawnIndex;
			}
#else
			entity->m_bIsVisible = false;
#endif


		}
#ifdef USE_CHAMS_VISIBILITY_BASED 
		CAnimatableSceneObjectDesc::UpdateVisiblePawnIndexes(visibleIndexes.data(), visibleCount);
#endif
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}


float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

int main() {
	
	SetConsoleTitle("PureLiquid CS2 External");
	
	I::Initialize();

#ifdef USE_CHAMS
	auto hLatexChamsMaterial = I::pMaterialSystem->CreateMaterial(CMaterialSystem2::GetLatexChams(), "LatexChamsMaterial");

	if (!hLatexChamsMaterial) {
		printf("Couldn't create Material!!\n");
		return 0;
	}

	if (!CAnimatableSceneObjectDesc::InstallRendererHook(nullptr)) {
		printf("Error Installing CAnimatableSceneObjectDesc hook!!\n");
		return 0;
	}

	CAnimatableSceneObjectDesc::SetChamsColor(0, 35, 255, 255);
	CAnimatableSceneObjectDesc::SetChamsMaterial(hLatexChamsMaterial);
	CAnimatableSceneObjectDesc::SetChamsEnabled(true);
#endif

	std::thread([]() {ReadEntititesThread();}).detach();

#ifdef USE_CREATE_MOVE
	I::pCsGoInput->HookCreateMove();
	
	std::thread([]() {Aimbot::AimbotThread();}).detach();
#endif
#ifdef USE_SILENT_AIM
	CMsgQAngleCpy::Hook();
	Aimbot::m_bUseSilentAim = true;
#endif
	auto pGameEntitySystem = I::pGameResourceService->GetGameEntitySystem();
	bool bAntiAimOn = false;

	panorama::CUI::ShowWelcomeMsg();

	while (!GetAsyncKeyState(VK_DELETE)) {

		if (!GetAsyncKeyState(VK_LSHIFT) && !GetAsyncKeyState(VK_RSHIFT)) {
			Sleep(100);
			continue;
			
		}

		continue;

		auto pLocalEntity = CGameEntitySystem::GetLocalPlayer();

		if (!pLocalEntity)
			continue;
		auto pRenderMesh0 = pLocalEntity->m_pPawn->GetCModel_Imp()->GetRenderMesh(0);
		printf("Hitbox0: %s\n", pRenderMesh0->GetBoneData()[0]->m_boneName->Get().c_str());

		Sleep(100);
		// I::pPanoramaUIEngine->GetCUIEngineSource2()->RunScript("$.Msg(\"Hello, world123!\");"); // Execute Script
		// printf("CMD: 0x%p\n", I::pCsGoInput->GetExecutionData().cmd);

	}
#ifdef USE_SILENT_AIM
	CMsgQAngleCpy::Unhook();
#endif
#ifdef USE_CREATE_MOVE
	I::pCsGoInput->UnhookCreateMove();
#endif
#ifdef USE_CHAMS
	CAnimatableSceneObjectDesc::UninstallRendererHook();
#endif

	panorama::CUI::ShowShutdownMsg();
	return 1;
}