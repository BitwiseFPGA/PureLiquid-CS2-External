#include <CS2/Managers/CModelManager.h>
#include <CS2/ExtendedSDK/resourcesystem/CModel_Imp.h>
#include <CS2/ExtendedSDK/modellib/CRenderMesh.h>
#include <CS2/SDK/modellib/RenderSkeletonBone_t.hpp>
#include <GlobalData/Include.h>

using namespace Globals;

namespace CS2 {
	
	CModelManager* pModelManager = CModelManager::Get();

	void CModelManager::CollectModelData(::CS2::resourcesystem::CModel_Imp* pModel) {
		if (!pModel)
			return;

		auto szModelName = pModel->GetModelName();

		if (szModelName.empty())
			return;

		printf("Collecting model data for %s!\n", szModelName.c_str());


		if (m_RegisteredModels.contains(szModelName))
			return;

		auto pRenderMesh0 = pModel->GetRenderMesh(0);

		if (!pRenderMesh0)
			return;
		auto vBones = pRenderMesh0->GetBoneData();
		if (!vBones.empty()) {

			OrderedBoneMap boneMap;
			BoneIndexMap boneIndexMap;

			int i = 0;
			for (int tempBoneIdx = 0; tempBoneIdx < vBones.size(); tempBoneIdx++) {
				auto boneNameByIndex = pModel->GetBoneName(tempBoneIdx);
				boneIndexMap[boneNameByIndex] = tempBoneIdx;

			}
			for (const auto& bone : vBones) {
				auto szBoneName = bone->m_boneName->Get();
				auto szParentBoneName = bone->m_parentName->Get();
				int boneId = boneIndexMap[szBoneName];
				int parentBoneId = boneIndexMap[szParentBoneName];
				RenderSkeletonBoneStatic_t boneData{ szBoneName, bone->m_parentName->Get(),boneId,parentBoneId };
				modellib::RenderSkeletonBoneWithIndex_t ext{ boneData ,boneId,parentBoneId };
				boneMap[szBoneName] = ext;
				i++;
			}

			m_ModelBoneCache[szModelName] = std::move(boneMap);
			m_ModelBoneIndexMap[szModelName] = std::move(boneIndexMap);

		}
		else {
			m_ModelBoneCache[szModelName] = {};

		}



		auto vHitboxes = pRenderMesh0->GetHitboxes();

		if (!vHitboxes.empty()) {

			std::map<BoneName, HitboxStatic> hitboxMap;

			for (const auto& hitbox : vHitboxes) {
				auto szBoneName = hitbox->m_sBoneName->Get();
				if (!m_ModelBoneCache.contains(szModelName))
					continue;

				if (!m_ModelBoneCache[szModelName].contains(szBoneName))
					continue;

				HitboxStatic hitboxData{ hitbox->m_name->Get(),szBoneName,hitbox->m_vMinBounds, hitbox->m_vMaxBounds, hitbox->m_flShapeRadius, hitbox->m_nHitBoxIndex, m_ModelBoneIndexMap[szModelName][szBoneName] };
				hitboxMap[szBoneName] = hitboxData;

				m_ModelHitboxCache[szModelName] = hitboxMap;
			}
		}
		// system("pause");
		m_RegisteredModels.insert(szModelName);
	}


	int CModelManager::GetBoneIndexByName(::CS2::resourcesystem::CModel_Imp* pModel, std::string szBoneName) {
		if (!pModel)
			return -1;

		auto szModelName = pModel->GetModelName();

		if (!m_RegisteredModels.contains(szModelName))
			CollectModelData(pModel);

		return GetBoneIndexByName(szModelName, szBoneName);
	}


	int CModelManager::GetBoneIndexByName(std::string szModelName, std::string szBoneName) {
		if (szModelName.empty())
			return -1;


		if (!m_RegisteredModels.contains(szModelName))
			return -1;

		return m_ModelBoneCache[szModelName].contains(szBoneName) ? m_ModelBoneCache[szModelName][szBoneName].m_iBoneId : -1;
	}


	HitboxAndBoneDataList_t CModelManager::GetHitboxAndBoneData(std::string szModelName) {

		if (szModelName.empty())
			return {};


		if (!m_RegisteredModels.contains(szModelName))
			return {};

		return { m_ModelBoneCache[szModelName], m_ModelHitboxCache[szModelName] };
	}

	HitboxAndBoneDataList_t CModelManager::GetHitboxAndBoneData(::CS2::resourcesystem::CModel_Imp* pModel) {
		if (!pModel)
			return {};

		auto szModelName = pModel->GetModelName();

		if (!m_RegisteredModels.contains(szModelName))
			CollectModelData(pModel);

		return GetHitboxAndBoneData(szModelName);

	}


	HitboxAndBoneData_t CModelManager::GetHitboxAndBoneData(std::string szModelName, int iBone) {
		if (szModelName.empty())
			return {};


		if (!m_RegisteredModels.contains(szModelName))
			return {};


		std::string szSelectedBoneName{};
		for (const auto& [boneName, boneData] : m_ModelBoneCache[szModelName]) {
			if (boneData.m_iBoneId == iBone) {
				szSelectedBoneName = boneName;
				break;
			}
		}

		if (szSelectedBoneName.empty())
			return {};

		return GetHitboxAndBoneData(szModelName, szSelectedBoneName);
	}
	HitboxAndBoneData_t CModelManager::GetHitboxAndBoneData(::CS2::resourcesystem::CModel_Imp* pModel, int iBone) {
		if (!pModel)
			return {};

		auto szModelName = pModel->GetModelName();

		if (!m_RegisteredModels.contains(szModelName))
			CollectModelData(pModel);

		return GetHitboxAndBoneData(szModelName, iBone);

	}


	HitboxAndBoneData_t CModelManager::GetHitboxAndBoneData(std::string szModelName, std::string szBoneName) {
		if (szModelName.empty())
			return {};


		if (szBoneName.empty())
			return {};

		return {
			m_ModelBoneCache[szModelName].contains(szBoneName) ? m_ModelBoneCache[szModelName][szBoneName] : ::CS2::modellib::RenderSkeletonBoneWithIndex_t{},
			m_ModelHitboxCache[szModelName].contains(szBoneName) ? m_ModelHitboxCache[szModelName][szBoneName] : ::CS2::HitboxStatic{}
		};
	}

	HitboxAndBoneData_t CModelManager::GetHitboxAndBoneData(::CS2::resourcesystem::CModel_Imp* pModel, std::string szBoneName) {
		if (!pModel)
			return {};

		auto szModelName = pModel->GetModelName();

		if (!m_RegisteredModels.contains(szModelName))
			CollectModelData(pModel);

		return GetHitboxAndBoneData(szModelName, szBoneName);
	}


	CModelManager* CModelManager::Get() {
		if (!pModelManager)
			pModelManager = new CModelManager();
		return pModelManager;
	}

}