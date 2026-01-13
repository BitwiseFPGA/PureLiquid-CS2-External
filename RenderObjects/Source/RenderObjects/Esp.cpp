#include <Overlay/RenderObject.h>
#include <CS2/Interfaces/CGameEntitySystem.h>
#include <CS2/Interfaces/Include.h>
#include <RenderObjects/Esp.h>
#include <CS2/SDK/client/C_CSPlayerPawn.hpp>
#include <CS2/SDK/modellib/CHitBox.hpp>
#include <CS2/ExtendedSDK/client/C_CSPlayerPawn.h>
#include <CS2/ExtendedSDK/client/CGameSceneNode.h>
#include <CS2/SDK/client/CCSPlayerController.hpp>
#include <CS2/Managers/CModelManager.h>
#include <Source2/CBaseHandle.h>
#include <Math/Vector.h>
#include <math/Matrix.h>
#undef min;
#undef max;
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <Overlay/View3D.h>
#include <RenderObjects/ViewMatrixResolver.h>

namespace RenderObjects {
	inline XMMATRIX ToXMMATRIX(const ViewMatrix_t& vm)
	{
		return XMMATRIX(
			vm.arrData[0][0], vm.arrData[1][0], vm.arrData[2][0], vm.arrData[3][0],
			vm.arrData[0][1], vm.arrData[1][1], vm.arrData[2][1], vm.arrData[3][1],
			vm.arrData[0][2], vm.arrData[1][2], vm.arrData[2][2], vm.arrData[3][2],
			vm.arrData[0][3], vm.arrData[1][3], vm.arrData[2][3], vm.arrData[3][3]
		);
	}

	void DrawText(ImDrawList* drawList, ImVec2 pos, ImU32 col, const char* fmt, ...)
	{
		char buf[256];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, sizeof(buf), fmt, args);
		va_end(args);

		drawList->AddText(pos, col, buf);
	}

	// Cache structure to avoid repeated lookups
	struct CachedHitboxData {
		int headBoneIdx = -1;
		int ankleBoneIdx = -1;
		bool valid = false;
		void* lastModelPtr = nullptr;
	};

	// Static cache - persists between frames
	static CachedHitboxData s_HitboxCache[65] = {};

	// Helper: Check if position is in view frustum (basic check)
	inline bool IsInFrustum(const Vector3& pos, const XMMATRIX& viewProj, float margin = 1.2f) {
		XMVECTOR worldPos = XMVectorSet(pos.x, pos.y, pos.z, 1.0f);
		XMVECTOR clipPos = XMVector4Transform(worldPos, viewProj);

		float w = XMVectorGetW(clipPos);
		if (w <= 0.01f) return false; // Behind camera

		float x = XMVectorGetX(clipPos) / w;
		float y = XMVectorGetY(clipPos) / w;

		return (x >= -margin && x <= margin && y >= -margin && y <= margin);
	}

	void Esp::Render(ImVec2 displaySize, ImVec2 cursorPos, bool bMouseReleased, View3D* view3D) {
		auto pDrawList = ImGui::GetForegroundDrawList();
		auto pEntitySystem = ::CS2::I::pGameResourceService->GetGameEntitySystem();
		auto ViewMatrix = ToXMMATRIX(::CS2::ViewMatrixResolver::GetViewMatrix());

		view3D->SetViewMatrix(ViewMatrix);

		XMMATRIX viewProj = ViewMatrix; 

		// Process entities
		for (int i = 1; i < 65; i++) {
			auto entity = &::CS2::CGameEntitySystem::vEntityList[i];

			if (!entity->m_bIsValid || !entity->m_bIsAlive)
				continue;

			if (!entity->m_pPawn)
				continue;

			if (entity->m_bIsLocalPlayer)
				continue;

			if (entity->m_iBoneCount < 0)
				continue;

			if (entity->headBoneIdx < 0)
				continue;

			auto& cache = s_HitboxCache[i];
			void* currentModelPtr = entity->m_pPawn->GetCModel_Imp();

			if (!cache.valid || cache.lastModelPtr != currentModelPtr) {
				auto pModelData = ::CS2::pModelManager->GetHitboxAndBoneData((::CS2::resourcesystem::CModel_Imp * )currentModelPtr);

				// Cache bone indices
				auto pHeadHbx = pModelData.m_Hitboxes["head_0"];
				auto pAnkleHbx = pModelData.m_Hitboxes["ankle_r"];

				cache.headBoneIdx = pHeadHbx.m_iBoneIdx;
				cache.ankleBoneIdx = pAnkleHbx.m_iBoneIdx;
				cache.lastModelPtr = currentModelPtr;
				cache.valid = true;
			}

			if (cache.headBoneIdx < 0 || cache.ankleBoneIdx < 0)
				continue;

			auto boneMatrices = entity->m_pPawn->GetGameSceneNodeExtended()->pGameSceneNodeBonePtr->m_Bones;
			auto vHeadPos = boneMatrices[cache.headBoneIdx].GetOrigin();
			auto vAnklePos = boneMatrices[cache.ankleBoneIdx].GetOrigin();

			Vector3 vCenter = { vHeadPos.x, vHeadPos.y, (vHeadPos.z + vAnklePos.z) / 2.0f };
			if (!IsInFrustum(vCenter, viewProj)) {
				continue;
			}

			float height = vHeadPos.z - vAnklePos.z;

			XMFLOAT4 borderColor = (entity->m_pPawn->m_iTeamNum == 2)
				? XMFLOAT4(235.f / 255.f, 35.f / 255.f, 0.f / 255.f, 1.0f)
				: XMFLOAT4(0.f / 255.f, 35.f / 255.f, 235.f / 255.f, 1.0f);

			Vector3 boxCenter = { vCenter.x, vCenter.y, vCenter.z + 2.0f };
			Vector3 boxSize = { 30.0f, 30.0f, height + 12.0f };

			// view3D->DrawBox(boxCenter, boxSize, { 0.35f, 0.35f, 0.35f, 0.2f });
			// view3D->DrawWireBox(boxCenter, boxSize, borderColor);

			// DrawHealthBarScreenAligned(view3D, {vHeadPos.x, vHeadPos.y, vHeadPos.z}, entity->m_pPawn->m_iMaxHealth, entity->m_pPawn->m_iHealth, {5.0f, 5.0f, height});
		}
	}

	void Esp::DrawHealthBarWorldLeft(View3D* view3D, const XMFLOAT3& entityPos, float maxHealth, float curHealth, const XMFLOAT3& size)
	{
		float proportion = curHealth / maxHealth;

		// Use world-space left direction (global -X axis)
		XMVECTOR leftDir = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);

		// Offset the bar from entity center
		float leftOffset = size.x * 0.5f + 0.5f;
		XMVECTOR entPos = XMLoadFloat3(&entityPos);
		XMVECTOR worldPos = entPos + leftDir * leftOffset;

		// Background bar (full height)
		XMFLOAT3 bgCenter;
		XMStoreFloat3(&bgCenter, worldPos);
		view3D->DrawBox({ bgCenter.x,bgCenter.y,bgCenter.z }, { size.x, size.y, size.z }, { 0.35f, 0.35f, 0.35f, 0.86f });

		// Filled portion (vertical, top aligned)
		float filledH = size.z * proportion;
		XMFLOAT3 filledCenter;
		XMStoreFloat3(&filledCenter, worldPos);
		filledCenter.z -= (size.z - filledH) / 2.0f;

		// Health color
		XMFLOAT4 fillColor;
		if (proportion > 0.5f) fillColor = { 96.f / 255, 246.f / 255, 113.f / 255, 1.0f };
		else if (proportion > 0.25f) fillColor = { 247.f / 255, 214.f / 255, 103.f / 255, 1.0f };
		else fillColor = { 255.f / 255, 95.f / 255, 95.f / 255, 1.0f };

		view3D->DrawBox({ filledCenter.x,filledCenter.y,filledCenter.z}, {size.x, size.y, filledH}, fillColor);
	}

	void Esp::DrawHealthBarScreenAligned(View3D* view3D, const XMFLOAT3& entityPos, float maxHealth, float curHealth, const XMFLOAT3& size)
	{
		float proportion = curHealth / maxHealth;

		// Extract camera right vector from view matrix
		XMMATRIX view = view3D->cpuView;
		XMVECTOR camRight = XMVectorSet(view.r[0].m128_f32[0], view.r[1].m128_f32[0], view.r[2].m128_f32[0], 0.0f);

		// Compute left offset relative to camera
		float leftOffset = size.x * 0.5f + 0.1f;
		XMVECTOR offset = -camRight * leftOffset;

		// Load entity position and apply offset
		XMVECTOR entPos = XMLoadFloat3(&entityPos);
		XMVECTOR worldPos = XMVectorAdd(entPos, offset);

		XMFLOAT3 barCenter;
		XMStoreFloat3(&barCenter, worldPos);

		float barWidth = size.x;
		float barDepth = size.y;
		float barHeight = size.z;

		// Draw background (full health)
		view3D->DrawBox({ barCenter.x,barCenter.y,barCenter.z}, {barWidth, barDepth, barHeight}, {0.35f, 0.35f, 0.35f, 0.86f});

		// Draw filled portion (shrink from top)
		float filledH = barHeight * proportion;
		Vector3 filledCenter = { barCenter.x,barCenter.y,barCenter.z };
		filledCenter.z -= (barHeight - filledH) / 2.0f;

		XMFLOAT4 fillColor;
		if (proportion > 0.5f) fillColor = { 96.f / 255, 246.f / 255, 113.f / 255, 1.0f };
		else if (proportion > 0.25f) fillColor = { 247.f / 255, 214.f / 255, 103.f / 255, 1.0f };
		else fillColor = { 255.f / 255, 95.f / 255, 95.f / 255, 1.0f };

		view3D->DrawBox(filledCenter, { barWidth, barDepth, filledH }, fillColor);
	}

	void Esp::DrawHealthBarWorld(View3D* view3D, Vector3 topPos, float MaxHealth, float CurrentHealth, Vector3 size)
	{
		float proportion = CurrentHealth / MaxHealth;

		float barWidth = size.x;
		float barDepth = size.y;
		float barHeight = size.z;

		// Filled portion (shrink from top)
		float filledH = barHeight * proportion;
		Vector3 filledCenter = { topPos.x, topPos.y, topPos.z };
		filledCenter.z -= (barHeight - filledH) / 2.0f;

		// Health color
		XMFLOAT4 fillColor;
		if (proportion > 0.5f) fillColor = { 96.f / 255, 246.f / 255, 113.f / 255, 0.10f };
		else if (proportion > 0.25f) fillColor = { 247.f / 255, 214.f / 255, 103.f / 255, 0.10f };
		else fillColor = { 255.f / 255, 95.f / 255, 95.f / 255, 0.10f };

		view3D->DrawBox(filledCenter, { barWidth, barDepth, filledH }, fillColor);
	}
}