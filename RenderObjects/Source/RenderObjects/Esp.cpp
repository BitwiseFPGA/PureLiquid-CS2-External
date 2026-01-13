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

	void Esp::Render(ImVec2 displaySize, ImVec2 cursorPos, bool bMouseReleased, View3D* view3D) {
		auto pDrawList = ImGui::GetForegroundDrawList();

		auto pEntitySystem = ::CS2::I::pGameResourceService->GetGameEntitySystem();
		auto ViewMatrix = ToXMMATRIX(::CS2::ViewMatrixResolver::GetViewMatrix());

		view3D->SetViewMatrix(ViewMatrix);
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
			auto pModelData = ::CS2::pModelManager->GetHitboxAndBoneData(entity->m_pPawn->GetCModel_Imp());
			auto pHeadHbx = pModelData.m_Hitboxes["head_0"];
			auto pAnkleHbx = pModelData.m_Hitboxes["ankle_r"];
			auto boneMatrices = entity->m_pPawn->GetGameSceneNodeExtended()->pGameSceneNodeBonePtr->m_Bones;
			auto vHeadPos = boneMatrices[pHeadHbx.m_iBoneIdx].GetOrigin();
			auto vAnklePos = boneMatrices[pAnkleHbx.m_iBoneIdx].GetOrigin();
			/*

			view3D->DrawBox(
			{ vHeadPos.x, vHeadPos.y, vHeadPos.z },
				{ 10.0f, 10.0f, 12.0f },
				{ 0, 1, 0, 0.2f });*/


			Vector3 barSize = { 5.0f, 5.0f, vHeadPos.z - vAnklePos.z }; // width x height
			Vector3 vHealthBarPos = { vHeadPos.x - 20.0f ,vHeadPos.y,vHeadPos.z - 20.0f };

			Vector3 vCenter = { vHeadPos.x, vHeadPos.y, (vHeadPos.z + vAnklePos.z) / 2.0f };
			float radius = 5.0f;
			float height = vHeadPos.z - vAnklePos.z;
			XMFLOAT4 color = { 0.35f, 0.35f, 0.35f, 0.2f }; // red
			XMFLOAT4 borderColor = (entity->m_pPawn->m_iTeamNum == 2)
				? XMFLOAT4(235.f / 255.f, 35.f / 255.f, 0.f / 255.f, 255.f / 255.f)
				: XMFLOAT4(0.f / 255.f, 35.f / 255.f, 235.f / 255.f, 255.f / 255.f);

			view3D->DrawBox({ vCenter.x, vCenter.y, vCenter.z + 2.0f }, { 30.0f,30.0f,height + 12.0f }, color);
			view3D->DrawWireBox({ vCenter.x, vCenter.y, vCenter.z + 2.0f }, { 30.0f,30.0f,height + 12.0f }, borderColor);
		}

	}

	void Esp::DrawHealthBarWorldLeft(View3D* view3D, const XMFLOAT3& entityPos, float maxHealth, float curHealth, const XMFLOAT3& size)
	{
		float proportion = curHealth / maxHealth;

		// Use world-space left direction (global -X axis)
		XMVECTOR leftDir = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f); // always left in world coordinates

		// Offset the bar from entity center
		float leftOffset = size.x * 0.5f + 0.5f; // add some spacing
		XMVECTOR entPos = XMLoadFloat3(&entityPos);
		XMVECTOR worldPos = entPos + leftDir * leftOffset;

		// Background bar (full height)
		XMFLOAT3 bgCenter;
		XMStoreFloat3(&bgCenter, worldPos);
		view3D->DrawBox(bgCenter, { size.x, size.y, size.z }, { 0.35f, 0.35f, 0.35f, 0.86f });

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

		view3D->DrawBox(filledCenter, { size.x, size.y, filledH }, fillColor);
	}



	void Esp::DrawHealthBarScreenAligned(View3D* view3D, const XMFLOAT3& entityPos, float maxHealth, float curHealth, const XMFLOAT3& size)
	{
		float proportion = curHealth / maxHealth;

		// 1. Extract camera right vector from view matrix (cpuView is untransposed)
		XMMATRIX view = view3D->cpuView;
		XMVECTOR camRight = XMVectorSet(view.r[0].m128_f32[0], view.r[1].m128_f32[0], view.r[2].m128_f32[0], 0.0f);

		// 2. Compute left offset relative to camera
		float leftOffset = size.x * 0.5f + 0.1f; // optional spacing
		XMVECTOR offset = -camRight * leftOffset;

		// 3. Load entity position and apply offset
		XMVECTOR entPos = XMLoadFloat3(&entityPos);
		XMVECTOR worldPos = XMVectorAdd(entPos, offset);

		XMFLOAT3 barCenter;
		XMStoreFloat3(&barCenter, worldPos);

		float barWidth = size.x;
		float barDepth = size.y;
		float barHeight = size.z;

		// 4. Draw background (full health)
		view3D->DrawBox(barCenter, { barWidth, barDepth, barHeight }, { 0.35f, 0.35f, 0.35f, 0.86f });

		// 5. Draw filled portion (shrink from top)
		float filledH = barHeight * proportion;
		XMFLOAT3 filledCenter = barCenter;
		filledCenter.z -= (barHeight - filledH) / 2.0f; // keep top aligned

		XMFLOAT4 fillColor;
		if (proportion > 0.5f) fillColor = { 96.f / 255, 246.f / 255, 113.f / 255, 1.0f };
		else if (proportion > 0.25f) fillColor = { 247.f / 255, 214.f / 255, 103.f / 255, 1.0f };
		else fillColor = { 255.f / 255, 95.f / 255, 95.f / 255, 1.0f };

		view3D->DrawBox(filledCenter, { barWidth, barDepth, filledH }, fillColor);
	}


	void Esp::DrawHealthBarWorld(View3D* view3D, Vector3 topPos, float MaxHealth, float CurrentHealth, Vector3 size)
	{
		float proportion = CurrentHealth / MaxHealth;

		float barWidth = size.x;       // horizontal
		float barDepth = size.y;         // thin depth
		float barHeight = size.z;       // vertical

		// Background box (full health)
		// view3D->DrawBox({ topPos.x,topPos.y,topPos.z}, {barWidth, barDepth, barHeight}, {0.35f, 0.35f, 0.35f, 0.86f});

		// Filled portion (shrink from top)
		float filledH = barHeight * proportion;
		XMFLOAT3 filledCenter = { topPos.x,topPos.y,topPos.z };
		filledCenter.z -= (barHeight - filledH) / 2.0f; // move down so top remains aligned

		// Health color
		XMFLOAT4 fillColor;
		if (proportion > 0.5f) fillColor = { 96.f / 255, 246.f / 255, 113.f / 255, 0.10f };
		else if (proportion > 0.25f) fillColor = { 247.f / 255, 214.f / 255, 103.f / 255, 0.10 };
		else fillColor = { 255.f / 255, 95.f / 255, 95.f / 255,0.10 };

		view3D->DrawBox(filledCenter, { barWidth, barDepth, filledH }, fillColor);
	}
}