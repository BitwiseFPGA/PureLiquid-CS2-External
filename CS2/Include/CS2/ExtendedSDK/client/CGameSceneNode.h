#pragma once
#include <CS2/Offsets/client/CSkeletonInstance.hpp>
#include <CS2/SDK/client/CGameSceneNode.hpp>
#include <GlobalData/Include.h>
namespace CS2 {
	namespace client {

		class CGameSceneNodeBonePtr {
		public:
			PROPERTY_ARRAY(m_Bones, Matrix2x4_t, 104, 0x0);

		};
		class CGameSceneNodeExtended : public CGameSceneNode {
		public:
			PROPERTY(pGameSceneNodeBonePtr, CGameSceneNodeBonePtr*, SchemaOffsets::client::CSkeletonInstance::m_modelState + 0x80);
		};
	}
}