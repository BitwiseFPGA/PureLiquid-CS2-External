#include <CS2/ExtendedSDK/modellib/CRenderMesh.h>
#include <CS2/SDK/modellib/RenderSkeletonBone_t.hpp>
#include <GlobalData/Include.h>
#include <vector>
using namespace Globals;

namespace CS2 {
	namespace modellib {
		
		std::vector<CHitBox*> CRenderMeshExtended::GetHitboxes() {
			auto pHitboxData = m_HitboxData;
			if (pHitboxData->GetSize() <= 0)
				return {};
			auto hitboxSet0 = m_HitboxData->ReinterpretPtrEntry<CRenderMeshHitboxUtlVector*>(0);
			auto pHitboxes = hitboxSet0->m_Hitboxes;
			return pHitboxes->ReinterpretPtrEntries<CHitBox*>();
		}

		std::vector<CS2::modellib::RenderSkeletonBone_t*> CRenderMeshExtended::GetBoneData() {
			
			auto pSkeleton = m_skeleton;
			auto pBonesUtlList = pSkeleton->m_bones;
			auto iBoneListSize = pBonesUtlList->GetSize();

			return pBonesUtlList->ReinterpretPtrEntries<RenderSkeletonBone_t*>();
		}

	}
}