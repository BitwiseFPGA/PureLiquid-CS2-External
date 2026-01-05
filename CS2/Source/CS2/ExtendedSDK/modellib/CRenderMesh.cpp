#include <CS2/ExtendedSDK/modellib/CRenderMesh.h>
#include <GlobalData/Include.h>
#include <vector>
using namespace Globals;

namespace CS2 {
	namespace modellib {
		std::vector<CHitBox*> CRenderMeshExtended::GetHitboxes()
		{
			auto pHitboxData = m_HitboxData;
			if (pHitboxData->GetSize() <= 0)
				return {};
			auto hitboxSet0 = reinterpret_cast<CRenderMeshHitboxUtlVector*>(m_HitboxData->GetPtrList()[0]);
			auto pHitboxes = hitboxSet0->m_Hitboxes;
			return pHitboxes->MapPtrEntries<CHitBox*>([](uintptr_t addr) -> CHitBox* {
				return reinterpret_cast<CHitBox*>(addr);
			});
		}
	}
}