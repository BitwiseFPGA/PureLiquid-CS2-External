#pragma once
#include <GlobalData/Include.h>
namespace CS2 {
	namespace modellib {
		class PermModelInfo_t;
		class CRenderMeshExtended;

	}
	namespace resourcesystem {
		class CModel_Imp {
		public:
			PROPERTY(m_iBoneCount, int, 0x178);
			::CS2::modellib::CRenderMeshExtended* GetRenderMesh(int idx);
			::CS2::modellib::PermModelInfo_t* GetPermModelInfo();
			std::string GetModelName();
			std::string GetBoneName(int boneIdx);
		};
	}
}