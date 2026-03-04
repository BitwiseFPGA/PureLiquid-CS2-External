#include <Features/Triggerbot.h>
#include <Features/Aimbot.h>
#include <Math/Ray.h>
#include <CS2/Managers/CModelManager.h>
#include <CS2/ExtendedSDK/client/C_CSPlayerPawn.h>
#include <CS2/SDK/modellib/CHitBox.hpp>
#include <CS2/ExtendedSDK/client/CGameSceneNode.h>
#include <CS2/Interfaces/Include.h>
#undef min
#undef max


inline bool IntersectRayCapsule(
    const Vector& rayOrigin,
    const Vector& rayDir,          // assumed normalized
    const Vector& capsuleStart,
    const Vector& capsuleEnd,
    float radius)
{
    Vector capsuleDir = capsuleEnd - capsuleStart;
    Vector toRay = rayOrigin - capsuleStart;

    const float e = capsuleDir.DotProduct(capsuleDir);
    if (e < 1e-8f) // Capsule is a point, degenerate case
    {
        float distSq = (rayOrigin - capsuleStart).LengthSqr();
        return distSq <= radius * radius;
    }

    // Precompute dot products
    float b = rayDir.DotProduct(capsuleDir);
    float c = rayDir.DotProduct(toRay);
    float f = capsuleDir.DotProduct(toRay);

    // a = rayDir.DotProduct(rayDir) = 1 if normalized; if not, compute it:
    // Using normalized rayDir saves this dot product and a branch
    const float a = 1.0f;

    float denom = a * e - b * b;
    float s = 0.f, t = 0.f;

    if (denom != 0.f)
    {
        s = (b * f - c * e) / denom;
        s = std::max(s, 0.0f); // Ray parameter should not be negative (no back intersection)
    }
    else
    {
        // Parallel or degenerate case: just clamp s to 0
        s = 0.0f;
    }

    // Clamp t between [0, 1] for capsule segment
    t = (b * s + f) / e;
    t = std::clamp(t, 0.0f, 1.0f);

    Vector closestRayPoint = rayOrigin + rayDir * s;
    Vector closestCapsulePoint = capsuleStart + capsuleDir * t;

    float distSq = (closestRayPoint - closestCapsulePoint).LengthSqr();
    return distSq <= radius * radius;
}
// https://www.unknowncheats.me/forum/counter-strike-2-a/704473-external-hitbox-triggerbot.html
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

        auto pLocalPawn = pLocalEntity->m_pPawn;

        // Compute ONCE outside the loop
        Vector vLocalPos = pLocalPawn->m_vOldOrigin + pLocalPawn->m_vecViewOffset;
        auto vViewAngles = CS2::I::pCsGoInput->vViewAngles;
        Vector viewDir = vViewAngles.AngleToDirection();

        for (int i = 1; i < 65; i++) {
            auto entity = &CS2::CGameEntitySystem::vEntityList[i];
            if (!entity->m_bIsAlive || !entity->m_bIsValid) continue;

            auto entityPawn = entity->m_pPawn;
            if (!entityPawn || entityPawn == pLocalPawn) continue;
            if (entity->m_pPawn->m_iTeamNum == pLocalPawn->m_iTeamNum && m_bIgnoreTeamMembers) continue;

            auto boneMatrix = entityPawn->GetGameSceneNodeExtended()->pGameSceneNodeBonePtr->m_Bones;
            if (boneMatrix.empty()) continue;
            auto vHeadMatrix = boneMatrix[Aimbot::targetBoneIdx];
            Vector vEnemyHead = vHeadMatrix.GetOrigin();
            if (!vEnemyHead.IsValid()) continue;

            auto targetInfo = Aimbot::GetTargetFOVAndDistance(vLocalPos, viewDir, vEnemyHead);
            if (targetInfo.fovDeg > 20.0f) continue;

            auto targetBoneHitbox = CS2::pModelManager->GetHitboxAndBoneData(
                entity->m_pPawn->GetCModel_Imp(), Aimbot::targetBoneIdx);

            if (!targetBoneHitbox.hitbox.m_iBoneIdx) continue;

            if (targetBoneHitbox.hitbox.m_flShapreRadius < 0.f || targetBoneHitbox.hitbox.m_flShapreRadius > 100.f)
                continue;

            float shapeRadius = targetBoneHitbox.hitbox.m_flShapreRadius;

            Matrix3x4_t bone_matrix = vHeadMatrix.TranslateToMatrix3x4();
            Vector minTransformed = targetBoneHitbox.hitbox.m_vMinBounds.Transform(bone_matrix);
            Vector maxTransformed = targetBoneHitbox.hitbox.m_vMaxBounds.Transform(bone_matrix);

            if (!IntersectRayCapsule(vLocalPos, viewDir, minTransformed, maxTransformed, shapeRadius))
                continue;

            printf("Shoot!!\n");
            Aimbot::ShootIfPossible(pLocalPawn, pLocalEntity->m_pController);
        }
    }
}