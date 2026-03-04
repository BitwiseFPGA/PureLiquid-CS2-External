#pragma once
#include <CS2/Interfaces/CGameEntitySystem.h>
#include <Math/Vector.h>
#include <Math/QAngle.h>
#include <Math/Matrix.h>

class Aimbot {
private:

	struct TargetInfo
	{
		float distance;
		float fovDeg;
		Matrix2x4_t targetBoneMatrix;
		CS2::Entity* entity;
	};

	inline static int iAimKey = 4;
	inline static bool bAuthoShoot = true;
	inline static int iTargetBone = 6;
	struct FOVResult
	{
		float fovDeg;
		float distance; 
	};
public:
	static Aimbot::FOVResult GetTargetFOVAndDistance(
		const Vector3& eyePos,
		const Vector3& viewDir,
		const Vector3& targetPos);


	inline static TargetInfo bestTarget{};
	inline static bool m_bUseSilentAim = false;
	inline static int bestTargetIdx = -1;
	inline static int targetBoneIdx = -1;
	inline static float flMaxFov = 20.0f;
	inline static float flMaxDist = 1000.0f;
	inline static bool m_bIgnoreTeamMembers = false;
	static void Run();
	static void ResetTarget();
	static void ShootIfPossible(CS2::client::C_CSPlayerPawnExtended* , CS2::client::CCSPlayerController*);
	static void AimbotThread();
};