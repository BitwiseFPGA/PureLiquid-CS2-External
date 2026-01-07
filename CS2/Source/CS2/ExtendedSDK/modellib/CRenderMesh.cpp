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
			auto hitboxSet0 = reinterpret_cast<CRenderMeshHitboxUtlVector*>(m_HitboxData->GetPtrList()[0]);
			auto pHitboxes = hitboxSet0->m_Hitboxes;
			return pHitboxes->MapPtrEntries<CHitBox*>([](uintptr_t addr) -> CHitBox* {
				return reinterpret_cast<CHitBox*>(addr);
			});
		}
		std::vector<CS2::modellib::RenderSkeletonBone_t*> CRenderMeshExtended::GetBoneData() {
			
			auto pSkeleton = m_skeleton;
			auto pBonesUtlList = pSkeleton->m_bones;
			auto iBoneListSize = pBonesUtlList->GetSize();

			return pBonesUtlList->MapPtrEntries<RenderSkeletonBone_t*>([](uintptr_t addr) -> RenderSkeletonBone_t* {
				return reinterpret_cast<RenderSkeletonBone_t*>(addr);
			});
		}
	}
}