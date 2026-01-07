#pragma once
#include <string>
#include <Math/Vector.h>
#include <map>
#include <unordered_set>

namespace CS2 {

	namespace resourcesystem {
		class CModel_Imp;
	}


	struct HitboxStatic {
		std::string m_szName;
		std::string m_szBoneName;
		Vector3 m_vMinBounds;
		Vector3 m_vMaxBounds;
		float m_flShapreRadius;
		uint16_t m_nHitBoxIndex;
		int m_iBoneIdx;
	};

	struct RenderSkeletonBoneStatic_t {
	public:
		std::string m_szBoneName;
		std::string m_szParentName;
		int boneIdx;
		int parentBoneIdx;
	};


	namespace modellib {
		class CHitBox;
		struct RenderSkeletonBoneWithIndex_t {
			::CS2::RenderSkeletonBoneStatic_t  m_Data{};
			int m_iBoneId = -1;
			int m_iParentBoneId = -1;
		};
	}


	struct HitboxAndBoneData_t {
		::CS2::modellib::RenderSkeletonBoneWithIndex_t bone{};
		::CS2::HitboxStatic hitbox{};
	};

	struct HitboxAndBoneDataList_t {
		std::map<std::string, ::CS2::modellib::RenderSkeletonBoneWithIndex_t> m_Bones{};
		std::map<std::string, ::CS2::HitboxStatic> m_Hitboxes{};
	};


	class CModelManager {
	private:
		enum Bones : int;
		using ModelName = std::string;
		using BoneName = std::string;

		inline static CModelManager* pModelManager;

		using OrderedBoneMap = std::map<BoneName, modellib::RenderSkeletonBoneWithIndex_t>;
		using BoneIndexMap = std::map<BoneName, int>;

		std::map<ModelName, std::map<BoneName, HitboxStatic>> m_ModelHitboxCache{};
		std::map<ModelName, OrderedBoneMap> m_ModelBoneCache{};
		std::map<ModelName, BoneIndexMap> m_ModelBoneIndexMap{};

		
		std::unordered_set<ModelName> m_RegisteredModels{};


		CModelManager() {
			new(&m_ModelHitboxCache) std::map<ModelName, std::map<BoneName, modellib::CHitBox>>{};
			new(&m_ModelBoneCache) std::map<ModelName, OrderedBoneMap>{};
			new(&m_ModelBoneIndexMap) std::map<ModelName, BoneIndexMap >{};
		}

		void CollectModelData(::CS2::resourcesystem::CModel_Imp* pModel);

	public:
		int GetBoneIndexByName(::CS2::resourcesystem::CModel_Imp* pModel, std::string szBoneName);
		int GetBoneIndexByName(std::string szModelName, std::string szBoneName);

		HitboxAndBoneDataList_t GetHitboxAndBoneData(std::string szModelName);
		HitboxAndBoneDataList_t GetHitboxAndBoneData(::CS2::resourcesystem::CModel_Imp* pModel);

		HitboxAndBoneData_t GetHitboxAndBoneData(std::string szModelName, int iBone);
		HitboxAndBoneData_t GetHitboxAndBoneData(::CS2::resourcesystem::CModel_Imp* pModel, int iBone);

		HitboxAndBoneData_t GetHitboxAndBoneData(std::string szModelName, std::string szBoneName);
		HitboxAndBoneData_t GetHitboxAndBoneData(::CS2::resourcesystem::CModel_Imp* pModel, std::string szBoneName);

	public:
		static CModelManager* Get();
	};
	extern CModelManager* pModelManager;
}