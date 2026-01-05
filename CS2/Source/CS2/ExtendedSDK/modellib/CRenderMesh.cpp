#include <GlobalData/Include.h>
#include <CS2/ExtendedSDK/modellib/CRenderMesh.h>
#include <CS2/SDK/modellib/CRenderSkeleton.hpp>
#include <CS2/Offsets/modellib/CRenderSkeleton.hpp>
#include <CS2/Offsets/modellib/CRenderMesh.hpp>
using namespace Globals;

namespace CS2 {
	namespace modellib {
		CRenderSkeleton* CRenderMeshExtended::GetRenderSkeleton() {
			return proc.ReadDirect<CRenderSkeleton*>(reinterpret_cast<uintptr_t>(this) + CS2::SchemaOffsets::modellib::CRenderMesh::m_skeleton);
		}
	}
}

