#include <SDK/modellib/CRenderMesh.hpp>
namespace CS2 {
	namespace modellib {
		class CRenderSkeleton;
	}
	namespace modellib {
		class CRenderMeshExtended : public CRenderMesh {
		public:
			CRenderSkeleton* GetRenderSkeleton();
		};
	}
}