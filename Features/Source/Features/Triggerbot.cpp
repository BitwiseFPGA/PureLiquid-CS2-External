#include <Features/Triggerbot.h>
#include <Features/Aimbot.h>
#include <Math/Ray.h>
#include <CS2/Managers/CModelManager.h>
#include <CS2/ExtendedSDK/client/C_CSPlayerPawn.h>
#include <CS2/SDK/modellib/CHitBox.hpp>
#include <CS2/ExtendedSDK/client/CGameSceneNode.h>
#include <CS2/Interfaces/Include.h>

void Triggerbot::Run() {
	while (true) {

		if (!GetAsyncKeyState(VK_LSHIFT)) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}

		auto pLocalEntity = CS2::CGameEntitySystem::GetLocalPlayer();

		if (!pLocalEntity || !Aimbot::targetBoneIdx) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}

		auto entity = Aimbot::bestTarget.entity;
		if (entity) {
			auto targetBoneHitbox = CS2::pModelManager->GetHitboxAndBoneData(entity->m_pPawn->GetCModel_Imp(), Aimbot::targetBoneIdx);

			if (targetBoneHitbox.hitbox.m_iBoneIdx) {
				auto obb = OBB{};
				obb.FromMatrixAndAABB(Aimbot::bestTarget.targetBoneMatrix, targetBoneHitbox.hitbox.m_vMinBounds, targetBoneHitbox.hitbox.m_vMaxBounds);

				Vector vecView = pLocalEntity->m_pPawn->m_vecViewOffset;

				Vector vLocalPos = pLocalEntity->m_pPawn->m_vOldOrigin + vecView;
				auto vViewAngles = CS2::I::pCsGoInput->vViewAngles;

				auto viewDir = vViewAngles.AngleToDirection();

				Ray ray(vLocalPos, viewDir);

				float distance;
				if (!ray.IntersectWithOBB(obb, distance))
					continue;

				if (distance <= 0)
					continue;
				printf("Shoot!!\n");
				Aimbot::ShootIfPossible(pLocalEntity->m_pPawn, pLocalEntity->m_pController);
			}
		}
		// std::this_thread::sleep_for(std::chrono::milliseconds(1));

	}
}